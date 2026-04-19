// SPDX-License-Identifier: GPL-2.0 OR MIT
/*
 * Malloc-based filesystem
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/statfs.h>
#include <linux/pagemap.h>
#include <linux/fs_context.h>

#include <libnetq/Malloc.h>
#include <libnetq/String.h>
#include <libnetq/List.h>
#include <libnetq/Mutex.h>
#include <libnetq/Atomic.h>
#include <libnetq/RefCount.h>
#include <libnetq/Log.h>

MODULE_AUTHOR("Yurii Yakubin <yurii.yakubin@gmail.com>");
MODULE_DESCRIPTION("Malloc-based filesystem");
MODULE_LICENSE("Dual MIT/GPL");

#define MALLOCFS_SUPER_MAGIC 0x20260412
#define MALLOCFS_DEFAULT_MODE 0755

#define MALLOCFS_NAMELEN 64

typedef struct NQMallocFsNode NQMallocFsNode;

typedef struct NQMallocFsCtx NQMallocFsCtx;
struct NQMallocFsCtx {
  NQMallocFsNode* root;
};

static void NQMallocFsCtx_init(NQMallocFsCtx* thiz)
{
  thiz->root = NULL;
}

static NQMallocFsCtx* NQMallocFsCtx_create(void)
{
  NQMallocFsCtx* thiz = (NQMallocFsCtx*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL)
    return NULL;

  NQMallocFsCtx_init(thiz);
  return thiz;
}

static void NQMallocFsCtx_finalize(NQMallocFsCtx* thiz)
{
}

static void NQMallocFsCtx_destroy(NQMallocFsCtx* thiz)
{
  NQMallocFsCtx_finalize(thiz);
  NQFree(thiz);
}

struct NQMallocFsNode {
  NQMutex mutex;
  NQAtomic32 openCount;
  NQListHead entries; // DirEntries or MemoryEntries
};

typedef struct NQMallocFsDirEntry NQMallocFsDirEntry;
struct NQMallocFsDirEntry {
  NQListHead list;
  NQRefCount refCount;
  uint8_t length;
  char characters[MALLOCFS_NAMELEN + 1];
  NQMallocFsNode* node;
};

static void NQMallocFsDirEntry_initUnsafe(NQMallocFsDirEntry* thiz, const char* name, size_t nlen)
{
  NQListHead_init(&thiz->list);
  NQRefCount_init(&thiz->refCount);
  memcpy(thiz->characters, name, nlen);
  thiz->length = nlen;
  thiz->characters[nlen] = 0;
  thiz->node = NULL;
}

static NQMallocFsDirEntry* NQMallocFsDirEntry_create(const char* name, size_t nlen)
{
  if (nlen > MALLOCFS_NAMELEN)
    return NULL;

  NQMallocFsDirEntry* thiz = (NQMallocFsDirEntry*)NQMalloc(sizeof(NQMallocFsDirEntry));
  if (thiz != NULL)
    NQMallocFsDirEntry_initUnsafe(thiz, name, nlen);

  return thiz;
}

static void NQMallocFsNode_init(NQMallocFsNode* thiz)
{
  NQMutex_init(&thiz->mutex);
  NQAtomic32_init(&thiz->openCount, 0);
  NQListHead_init(&thiz->entries);
}

static void NQMallocFsNode_finalize(NQMallocFsNode* thiz)
{
  // Finalize thiz->entries
  NQMutex_destroy(&thiz->mutex);
}

static NQMallocFsDirEntry* NQMallocFsNode_findDirEntry(const NQMallocFsNode* thiz, const char* name, size_t nlen)
{
  NQListHead* iter = thiz->entries.next;
  while (iter != &thiz->entries) {
    NQMallocFsDirEntry* entry = NQ_CONTAINER_OF(iter, struct NQMallocFsDirEntry, list);
    if (entry->length == nlen && memcmp(entry->characters, name, nlen) == 0)
      return entry;
    iter = iter->next;
  }

  return NULL;
}

typedef struct mallocfs_context_struct {
  struct NQMallocFsCtx impl;
  umode_t mount_mode;
} mallocfs_context_t;

typedef struct mallocfs_node_struct {
  struct NQMallocFsNode impl;
  struct inode vfs_inode;
} mallocfs_node_t;

static struct kmem_cache* mallocfs_inode_cache;

static const struct inode_operations mallocfs_link_ops;
static const struct inode_operations mallocfs_inode_ops;
static const struct file_operations mallocfs_file_ops;
static const struct address_space_operations mallocfs_address_space_ops;
static const struct inode_operations mallocfs_dir_iops;
static const struct file_operations mallocfs_dir_fops;

static inline mallocfs_node_t* MALLOCFS_I(struct inode* inode)
{
  return list_entry(inode, struct mallocfs_node_struct, vfs_inode);
}

static inline unsigned mallocfs_node_type(const mallocfs_node_t* node)
{
  return fs_umode_to_dtype(node->vfs_inode.i_mode);
}

static inline unsigned long mallocfs_node_ino(const mallocfs_node_t* node)
{
  return &node->impl;
}

static struct inode* mallocfs_iget(struct super_block* sb, struct inode* dir, umode_t mode)
{
  struct inode* inode = new_inode(sb);
  if (!inode)
    return ERR_PTR(-ENOMEM);

  mallocfs_node_t* node = MALLOCFS_I(inode);

  set_nlink(inode, S_ISDIR(mode) ? 2 : 1);
  inode->i_ino = mallocfs_node_ino(node);

  if (dir != NULL)
    inode_init_owner(&nop_mnt_idmap, inode, dir, mode);
  else
    inode->i_mode = mode;

  simple_inode_init_ts(inode);
  switch (mode & S_IFMT) {
  case S_IFLNK:
    inode->i_op = &mallocfs_link_ops;
    break;
  case S_IFREG:
    inode->i_op = &mallocfs_inode_ops;
    inode->i_fop = &mallocfs_file_ops;
    inode->i_mapping->a_ops = &mallocfs_address_space_ops;
    break;
  case S_IFDIR:
    inode->i_op = &mallocfs_dir_iops;
    inode->i_fop = &mallocfs_dir_fops;
    break;
  default:
    return ERR_PTR(-EIO);
  }
  
  insert_inode_hash(inode);
  mark_inode_dirty(inode);
  NQ_LOGI("mallocfs_iget %p mode %o", node, inode->i_mode);
  return inode;
}

static const char* mallocfs_get_link(struct dentry *dentry, struct inode *inode, struct delayed_call *done)
{
  NQ_LOGI("mallocfs_get_link");

  char *link;
  if (!dentry)
    return ERR_PTR(-ECHILD);

  link = kmalloc(PATH_MAX, GFP_KERNEL);
  if (link) {
    char *path = NULL; // dentry_name(dentry);
    int err = -ENOMEM;
    if (path) {
      // err = hostfs_do_readlink(path, link, PATH_MAX);
      if (err == PATH_MAX)
        err = -E2BIG;
        // __putname(path);
    }
    if (err < 0) {
      kfree(link);
      return ERR_PTR(err);
    }
  }
  else {
    return ERR_PTR(-ENOMEM);
  }

  set_delayed_call(done, kfree_link, link);
  return link;
}

static const struct inode_operations mallocfs_link_ops = {
  .get_link = mallocfs_get_link,
};

static int mallocfs_setattr(struct mnt_idmap* idmap, struct dentry* dentry, struct iattr* attr)
{
  NQ_LOGI("mallocfs_setattr");
  return 0;
}

static const struct inode_operations mallocfs_inode_ops = {
  .setattr = mallocfs_setattr,
  .getattr = simple_getattr,
};

static int mallocfs_open(struct inode* inode, struct file* file)
{
  NQ_LOGI("mallocfs_open %s", file->f_path.dentry->d_name.name);
	mallocfs_node_t* node = MALLOCFS_I(inode);
  NQAtomic32_inc(&node->impl.openCount);
	return 0;
}

static int mallocfs_file_release(struct inode* inode, struct file* file)
{
  NQ_LOGI("mallocfs_file_release %s", file->f_path.dentry->d_name.name);
	mallocfs_node_t* node = MALLOCFS_I(inode);
  filemap_write_and_wait(inode->i_mapping);
  NQAtomic32_dec(&node->impl.openCount);
  return 0;
}

static int mallocfs_file_fsync(struct file* file, loff_t start, loff_t end, int datasync)
{
  NQ_LOGI("mallocfs_file_fsync");
  struct inode* inode = file->f_mapping->host;
  return file_write_and_wait_range(file, start, end);
}

static const struct file_operations mallocfs_file_ops = {
  .llseek = generic_file_llseek,
  .read_iter = generic_file_read_iter,
  .write_iter = generic_file_write_iter,
  .open = mallocfs_open,
  .release = mallocfs_file_release,
  .fsync = mallocfs_file_fsync,
  .splice_read = filemap_splice_read,
  .splice_write = iter_file_splice_write,
  .mmap_prepare = generic_file_mmap_prepare,
};

static int mallocfs_read_folio(struct file *file, struct folio *folio)
{
  NQ_LOGI("mallocfs_read_folio");
  char* buffer;
  loff_t start = folio_pos(folio);
  int bytes_read, ret = 0;

  buffer = kmap_local_folio(folio, 0);
  bytes_read = 0; // read_file(MALLOCFS_I(file)->fd, &start, buffer, PAGE_SIZE);
  if (bytes_read < 0)
    ret = bytes_read;
  else
    buffer = folio_zero_tail(folio, bytes_read, buffer + bytes_read);
  kunmap_local(buffer);

  folio_end_read(folio, ret == 0);
  return ret;
}

static int mallocfs_writepages(struct address_space* mapping, struct writeback_control* wbc)
{
  NQ_LOGI("mallocfs_writepages");
  struct inode* inode = mapping->host;
  struct folio *folio = NULL;
  loff_t i_size = i_size_read(inode);
  int err = 0;

  while ((folio = writeback_iter(mapping, wbc, folio, &err))) {
    loff_t pos = folio_pos(folio);
    size_t count = folio_size(folio);
    char* buffer;
    int ret;

    if (count > i_size - pos)
      count = i_size - pos;

    buffer = kmap_local_folio(folio, 0);
    ret = 0; // write_file(HOSTFS_I(inode)->fd, &pos, buffer, count);
    kunmap_local(buffer);
    folio_unlock(folio);
    if (ret != count) {
      err = ret < 0 ? ret : -EIO;
      mapping_set_error(mapping, err);
    }
  }

	return err;
}

static int mallocfs_write_begin(const struct kiocb *iocb, struct address_space *mapping,
                                loff_t pos, unsigned len, struct folio** foliop, void** fsdata)
{
  NQ_LOGI("mallocfs_write_begin");
  pgoff_t index = pos >> PAGE_SHIFT;

  *foliop = __filemap_get_folio(mapping, index, FGP_WRITEBEGIN, mapping_gfp_mask(mapping));
  if (IS_ERR(*foliop))
    return PTR_ERR(*foliop);

  return 0;
}

static int mallocfs_write_end(const struct kiocb *iocb, struct address_space *mapping,
                            loff_t pos, unsigned len, unsigned copied, struct folio *folio, void *fsdata)
{
  NQ_LOGI("mallocfs_write_end");
  struct inode *inode = mapping->host;
  void *buffer;
  size_t from = offset_in_folio(folio, pos);
  int err;

  buffer = kmap_local_folio(folio, from);
  err = 0; // write_file(FILE_HOSTFS_I(iocb->ki_filp)->fd, &pos, buffer, copied);
  kunmap_local(buffer);

  if (!folio_test_uptodate(folio) && err == folio_size(folio))
  folio_mark_uptodate(folio);

  /*
  * If err > 0, write_file has added err to pos, so we are comparing
  * i_size against the last byte written.
  */
  if (err > 0 && (pos > inode->i_size))
  inode->i_size = pos;
  folio_unlock(folio);
  folio_put(folio);

  return err;
}

static const struct address_space_operations mallocfs_address_space_ops = {
  .read_folio	= mallocfs_read_folio,
  .writepages 	= mallocfs_writepages,
  .dirty_folio	= filemap_dirty_folio,
  .write_begin	= mallocfs_write_begin,
  .write_end	= mallocfs_write_end,
  .migrate_folio	= filemap_migrate_folio,
};

static struct inode* mallocfs_alloc_inode(struct super_block* sb)
{
  NQ_LOGI("> mallocfs_alloc_inode");
	mallocfs_node_t* node;

	node = alloc_inode_sb(sb, mallocfs_inode_cache, GFP_KERNEL_ACCOUNT);
	if (node == NULL)
		return NULL;

	inode_init_once(&node->vfs_inode);
  NQMallocFsNode_init(&node->impl);

  NQ_LOGI("< mallocfs_alloc_inode %p", node);
	return &node->vfs_inode;
}

static void mallocfs_evict_inode(struct inode* inode)
{
  mallocfs_node_t* node = MALLOCFS_I(inode);
  NQ_LOGI("mallocfs_evict_inode %p ino=%lu nlink=%u i_count=%i", node, inode->i_ino, inode->i_nlink, icount_read(inode));
  truncate_inode_pages_final(&inode->i_data);
  clear_inode(inode);

  // TODO: DROP enties
	// 
  // NQMallocFsNode_finalize(&node->impl);
}

static void mallocfs_free_inode(struct inode* inode)
{
	mallocfs_node_t* node = MALLOCFS_I(inode);
  NQ_LOGI("mallocfs_free_inode %p i_count %i", node, icount_read(inode));

  NQMallocFsNode_finalize(&node->impl);
  kmem_cache_free(mallocfs_inode_cache, node);
}

static int mallocfs_statfs(struct dentry* dentry, struct kstatfs* buf)
{
  NQ_LOGI("mallocfs_statfs");
  struct super_block* sb = dentry->d_sb;
  NQMallocFsCtx* ctx = (NQMallocFsCtx*)sb->s_fs_info;

  /*buf->f_type = dentry->d_sb->s_magic;
  buf->f_bsize = dentry->d_sb->s_blocksize;
  buf->f_namelen;
  buf->f_blocks = sbi->total_groups *	(sbi->group_size - sbi->inode_blocks);
  buf->f_bavail = sbi->data_free;
	buf->f_bfree = sbi->data_free;
  buf->f_files;
  buf->f_fsid;*/

  return 0;
}

static const struct super_operations mallocfs_super_ops = {
  .alloc_inode = mallocfs_alloc_inode,
  .free_inode = mallocfs_free_inode,
  .drop_inode = inode_just_drop,
  .evict_inode = mallocfs_evict_inode,
  .statfs = mallocfs_statfs,
};

static struct dentry* mallocfs_lookup(struct inode* dir, struct dentry* dentry, unsigned int flags)
{
  NQ_LOGI("mallocfs_lookup %*.s", dentry->d_name.len, dentry->d_name.name);

  if (dentry->d_name.len > MALLOCFS_NAMELEN) {
    return ERR_PTR(-ENAMETOOLONG);
  }

  struct inode* inode = NULL;

  mallocfs_node_t* parent = MALLOCFS_I(dir);
  NQMutex_lock(&parent->impl.mutex);
  NQMallocFsDirEntry* entry = NQMallocFsNode_findDirEntry(&parent->impl, dentry->d_name.name, dentry->d_name.len);
  if (entry) {
    // do refcount to entry
  }
  NQMutex_unlock(&parent->impl.mutex);

  if (entry) {
    mallocfs_node_t* info = NQ_CONTAINER_OF(entry->node, struct mallocfs_node_struct, impl);
    inode = &info->vfs_inode;
    dentry->d_fsdata = entry;
  }

  return d_splice_alias(inode, dentry);
}

static int mallocfs_create(struct mnt_idmap* idmap, struct inode* dir, struct dentry* dentry, umode_t mode, bool excl)
{
  NQ_LOGI("mallocfs_create");
  return -ENOMEM;
}

static int mallocfs_link(struct dentry* to, struct inode* inode, struct dentry *from)
{
  NQ_LOGI("mallocfs_link");
  return -ENOMEM;
}

static int mallocfs_unlink(struct inode* inode, struct dentry* dentry)
{
  NQ_LOGI("mallocfs_unlink");
  return -ENOMEM;
}

static int mallocfs_symlink(struct mnt_idmap* idmap, struct inode* inode, struct dentry* dentry, const char* to)
{
  NQ_LOGI("mallocfs_symlink");
  return -ENOMEM;
}

static struct dentry* mallocfs_mkdir(struct mnt_idmap* idmap, struct inode* dir, struct dentry* dentry, umode_t mode)
{
  NQ_LOGI("> mallocfs_mkdir %*.s", dentry->d_name.len, dentry->d_name.name);
  if (dentry->d_fsdata != NULL) {
    NQ_LOGE("Wrong dentry");
    return ERR_PTR(-EIO);
  }

  NQMallocFsDirEntry* entry = NQMallocFsDirEntry_create(dentry->d_name.name, dentry->d_name.len); // From cache and move to lookup
  if (entry == NULL) {
    NQ_LOGE("< mallocfs_mkdir");
    return ERR_PTR(-ENOMEM);
  }

  struct super_block* sb = dentry->d_sb;
  struct inode* inode = mallocfs_iget(sb, dir, mode | S_IFDIR);
  if (IS_ERR(inode)) {
    NQFree(entry);
    return ERR_CAST(inode);
  }
  entry->node = &MALLOCFS_I(inode)->impl;

  dentry->d_fsdata = entry;

  inc_nlink(dir);

  mallocfs_node_t* parent = MALLOCFS_I(dir);
  NQMutex_lock(&parent->impl.mutex);
  NQListHead_addBack(&parent->impl.entries, &entry->list);
  NQMutex_unlock(&parent->impl.mutex);

  // d_instantiate(dentry, inode);
  d_make_persistent(dentry, inode);

  NQ_LOGI("< mallocfs_mkdir");
  return NULL;
}

static int mallocfs_rmdir(struct inode* dir, struct dentry* dentry)
{
  NQ_LOGI("mallocfs_rmdir");
  if (dentry->d_fsdata == NULL) {
    NQ_LOGE("Wrong dentry");
    return -EIO;
  }

  struct inode* inode = d_inode(dentry);
  mallocfs_node_t* info = MALLOCFS_I(inode);

  if (!NQListHead_isEmpty(&info->impl.entries))
    return -ENOTEMPTY;

  int err = -EIO;
  mallocfs_node_t* parent = MALLOCFS_I(dir);
  NQMutex_lock(&parent->impl.mutex);
  NQListHead* iter = parent->impl.entries.next;
  while (iter != &parent->impl.entries) {
    NQMallocFsDirEntry* entry = NQ_CONTAINER_OF(iter, struct NQMallocFsDirEntry, list);
    if (entry == dentry->d_fsdata) {
      NQListHead_remove(&entry->list);
      err = 0;
      break;
    }
    iter = iter->next;
  }
  NQMutex_unlock(&parent->impl.mutex);

  if (err) {
    NQ_LOGE("Can't find entry");
    return err;
  }

  NQMallocFsDirEntry* entry = dentry->d_fsdata;
  NQFree(entry);
  dentry->d_fsdata = NULL;

  d_drop(dentry);
  d_make_discardable(dentry);
  clear_nlink(inode);

  if (dir->i_nlink)
    drop_nlink(dir);

  return 0;
}

static int mallocfs_mknod(struct mnt_idmap* idmap, struct inode* dir, struct dentry* dentry, umode_t mode, dev_t dev)
{
  NQ_LOGI("mallocfs_mknod");
  return -ENOMEM;
}

static int mallocfs_rename(struct mnt_idmap* idmap, struct inode* old_dir, struct dentry* old_dentry,
                          struct inode* new_dir, struct dentry* new_dentry, unsigned int flags)
{
  NQ_LOGI("mallocfs_rename");
  return -ENOMEM;
}

static const struct inode_operations mallocfs_dir_iops = {
  .lookup = mallocfs_lookup,
  .create = mallocfs_create,
  .link = mallocfs_link,
  .unlink = mallocfs_unlink,
  .symlink = mallocfs_symlink,
  .mkdir = mallocfs_mkdir,
  .rmdir = mallocfs_rmdir,
  .mknod = mallocfs_mknod,
  .rename = mallocfs_rename,
  .setattr = mallocfs_setattr,
};

static int mallocfs_readdir(struct file* file, struct dir_context* ctx)
{
  NQ_LOGI("mallocfs_readdir");

  if (ctx->pos == 0) {
    if (!dir_emit_dot(file, ctx))
      return 0;
    ctx->pos++;
  }

	if (ctx->pos == 1) {
		if (!dir_emit_dotdot(file, ctx))
      return 0;
		ctx->pos++;
	}

  loff_t pos = 2;
	mallocfs_node_t* node = MALLOCFS_I(file->f_inode);
  NQMutex_lock(&node->impl.mutex);
  NQListHead* iter = node->impl.entries.next;
  while (iter != &node->impl.entries) {
    if (ctx->pos == pos) {
      NQMallocFsDirEntry* entry = NQ_CONTAINER_OF(iter, struct NQMallocFsDirEntry, list);
      mallocfs_node_t* iter_node = NQ_CONTAINER_OF(entry->node, struct mallocfs_node_struct, impl);
      unsigned type = mallocfs_node_type(iter_node);
      unsigned long ino = mallocfs_node_ino(iter_node);
      if (!dir_emit(ctx, entry->characters, entry->length, ino, type))
        break;
      ctx->pos++;
    }
    iter = iter->next;
    pos++;
  }
  NQMutex_unlock(&node->impl.mutex);

  return 0;
}

static const struct file_operations mallocfs_dir_fops = {
	.llseek = generic_file_llseek,
	.read = generic_read_dir,
	.iterate_shared = mallocfs_readdir,
	.open = mallocfs_open,
	.fsync = mallocfs_file_fsync,
};

static int mallocfs_fill_super(struct super_block* sb, struct fs_context* fc)
{
  NQ_LOGI("mallocfs_fill_super");
  int ret;
  struct inode* inode;
  mallocfs_context_t* ctx = sb->s_fs_info;
  
  sb->s_blocksize_bits = PAGE_SHIFT;
  sb->s_blocksize = PAGE_SIZE;
  sb->s_maxbytes = MAX_LFS_FILESIZE;
  sb->s_op = &mallocfs_super_ops;
  sb->s_magic = MALLOCFS_SUPER_MAGIC;
  sb->s_d_flags = DCACHE_DONTCACHE;
  sb->s_time_gran = 1;

  ret = super_setup_bdi(sb);
  if (ret)
    return ret;

  inode = mallocfs_iget(sb, NULL, S_IFDIR | ctx->mount_mode);
  if (IS_ERR(inode))
    return PTR_ERR(inode);

  sb->s_root = d_make_root(inode);
  if (sb->s_root == NULL) {
    return -ENOMEM;
  }

	mallocfs_node_t* node = MALLOCFS_I(inode);
  ctx->impl.root = &node->impl;
	return 0;
}

static int mallocfs_get_tree(struct fs_context* fc)
{
  return get_tree_nodev(fc, mallocfs_fill_super);
}

static void mallocfs_free(struct fs_context* fc)
{
  NQ_LOGI("mallocfs_free");
}

static const struct fs_context_operations mallocfs_fs_context_ops = {
  .free = mallocfs_free,
  .get_tree = mallocfs_get_tree,
};

static int mallocfs_init_fs_context(struct fs_context* fc)
{
  NQ_LOGI("mallocfs_init_fs_context");
  mallocfs_context_t* ctx = (mallocfs_context_t*)NQMalloc(sizeof(*ctx));
  if (ctx == NULL)
    return -EINVAL;

  NQMallocFsCtx_init(&ctx->impl);
  ctx->mount_mode = MALLOCFS_DEFAULT_MODE;

  fc->s_fs_info = ctx;
  fc->ops = &mallocfs_fs_context_ops;

  return 0;
}

static void mallocfs_kill_sb(struct super_block* sb)
{
  NQ_LOGI("mallocfs_kill_sb");
  mallocfs_context_t* ctx = sb->s_fs_info;

	kill_anon_super(sb);

  NQMallocFsCtx_finalize(&ctx->impl);
  NQFree(ctx);
}

static struct file_system_type mallocfs_type = {
  .owner           = THIS_MODULE,
  .name            = "mallocfs",
  .init_fs_context = mallocfs_init_fs_context,
  .kill_sb         = mallocfs_kill_sb,
  .fs_flags        = FS_USERNS_MOUNT,
};

MODULE_ALIAS_FS("mallocfs");

static int __init init_mallocfs(void)
{
  mallocfs_inode_cache = KMEM_CACHE(mallocfs_node_struct, 0);
  if (!mallocfs_inode_cache)
    return -ENOMEM;
	return register_filesystem(&mallocfs_type);
}

static void __exit exit_mallocfs(void)
{
  unregister_filesystem(&mallocfs_type);
  kmem_cache_destroy(mallocfs_inode_cache);
}

module_init(init_mallocfs)
module_exit(exit_mallocfs)

// mount -t mallocfs none /opt/work/temp/mallocfs
// umount /opt/work/temp/mallocfs
// cd /opt/work/temp/mallocfs
// mkdir /opt/work/temp/mallocfs/123
// rmdir /opt/work/temp/mallocfs/123
