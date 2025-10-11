/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASCII_H
#define _LIBNETQ_ASCII_H

// <char> <url-type>
#define NQ_FOR_EACH_ASCII_TABLE(macro) \
  macro('\x00', Other)  \
  macro('\x01', Other)  \
  macro('\x02', Other)  \
  macro('\x03', Other)  \
  macro('\x04', Other)  \
  macro('\x05', Other)  \
  macro('\x06', Other)  \
  macro('\x07', Other)  \
  macro('\b',   Other)  \
  macro('\t',   Other)  \
  macro('\n',   Other)  \
  macro('\v',   Other)  \
  macro('\f',   Other)  \
  macro('\r',   Other)  \
  macro('\x0E', Other)  \
  macro('\x0F', Other)  \
  macro('\x10', Other)  \
  macro('\x11', Other)  \
  macro('\x12', Other)  \
  macro('\x13', Other)  \
  macro('\x14', Other)  \
  macro('\x15', Other)  \
  macro('\x16', Other)  \
  macro('\x17', Other)  \
  macro('\x18', Other)  \
  macro('\x19', Other)  \
  macro('\x1A', Other)  \
  macro('\x1B', Other)  \
  macro('\x1C', Other)  \
  macro('\x1D', Other)  \
  macro('\x1E', Other)  \
  macro('\x1F', Other)  \
  macro(' ',    Other)  \
  macro('!',    Reserved)  \
  macro('"',    Other)  \
  macro('#',    Reserved)  \
  macro('$',    Reserved)  \
  macro('%',    Other)  \
  macro('&',    Reserved)  \
  macro('\'',   Reserved)  \
  macro('(',    Reserved)  \
  macro(')',    Reserved)  \
  macro('*',    Reserved)  \
  macro('+',    Reserved)  \
  macro(',',    Reserved)  \
  macro('-',    Unreserved)  \
  macro('.',    Unreserved)  \
  macro('/',    Reserved)  \
  macro('0',    Unreserved)  \
  macro('1',    Unreserved)  \
  macro('2',    Unreserved)  \
  macro('3',    Unreserved)  \
  macro('4',    Unreserved)  \
  macro('5',    Unreserved)  \
  macro('6',    Unreserved)  \
  macro('7',    Unreserved)  \
  macro('8',    Unreserved)  \
  macro('9',    Unreserved)  \
  macro(':',    Reserved)  \
  macro(';',    Reserved)  \
  macro('<',    Other)  \
  macro('=',    Reserved)  \
  macro('>',    Other)  \
  macro('?',    Reserved)  \
  macro('@',    Reserved)  \
  macro('A',    Unreserved)  \
  macro('B',    Unreserved)  \
  macro('C',    Unreserved)  \
  macro('D',    Unreserved)  \
  macro('E',    Unreserved)  \
  macro('F',    Unreserved)  \
  macro('G',    Unreserved)  \
  macro('H',    Unreserved)  \
  macro('I',    Unreserved)  \
  macro('J',    Unreserved)  \
  macro('K',    Unreserved)  \
  macro('L',    Unreserved)  \
  macro('M',    Unreserved)  \
  macro('N',    Unreserved)  \
  macro('O',    Unreserved)  \
  macro('P',    Unreserved)  \
  macro('Q',    Unreserved)  \
  macro('R',    Unreserved)  \
  macro('S',    Unreserved)  \
  macro('T',    Unreserved)  \
  macro('U',    Unreserved)  \
  macro('V',    Unreserved)  \
  macro('W',    Unreserved)  \
  macro('X',    Unreserved)  \
  macro('Y',    Unreserved)  \
  macro('Z',    Unreserved)  \
  macro('[',    Other)  \
  macro('\\',   Other)  \
  macro(']',    Other)  \
  macro('^',    Other)  \
  macro('_',    Unreserved)  \
  macro('`',    Other)  \
  macro('a',    Unreserved)  \
  macro('b',    Unreserved)  \
  macro('c',    Unreserved)  \
  macro('d',    Unreserved)  \
  macro('e',    Unreserved)  \
  macro('f',    Unreserved)  \
  macro('g',    Unreserved)  \
  macro('h',    Unreserved)  \
  macro('i',    Unreserved)  \
  macro('j',    Unreserved)  \
  macro('k',    Unreserved)  \
  macro('l',    Unreserved)  \
  macro('m',    Unreserved)  \
  macro('n',    Unreserved)  \
  macro('o',    Unreserved)  \
  macro('p',    Unreserved)  \
  macro('q',    Unreserved)  \
  macro('r',    Unreserved)  \
  macro('s',    Unreserved)  \
  macro('t',    Unreserved)  \
  macro('u',    Unreserved)  \
  macro('v',    Unreserved)  \
  macro('w',    Unreserved)  \
  macro('x',    Unreserved)  \
  macro('y',    Unreserved)  \
  macro('z',    Unreserved)  \
  macro('{',    Other)  \
  macro('|',    Other)  \
  macro('}',    Other)  \
  macro('~',    Unreserved)  \
  macro('\x7F', Other)

#define NQ_ASCII_TABLE_SIZE (128)

#endif /* _LIBNETQ_ASCII_H */
