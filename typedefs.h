#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

typedef unsigned char byte;

int compress(byte* pDest, long* pnDestLen, const byte* pSrc, long nSrcLen);
int decompress(byte* pDest, long* pnDestLen, const byte* pSrc, long nSrcLen);

#endif /* __TYPEDEFS_H__ */
