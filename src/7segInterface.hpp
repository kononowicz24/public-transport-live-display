#ifndef _7SEGINTERFACE_H_
#define _7SEGINTERFACE_H_

const int segments[] = {0x11, 0x22, 0x44, 0x88};
const int digits[] = {0xfb, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};

void digitShow(int a, int i);

#endif