#define REG_DISPLAY_VCOUNT (*((volatile unsigned short *)0x04000006))
#define REG_DMA3CNT_H      (*((volatile unsigned short *)0x040000DE))
#define REG_KEY_INPUT      (*((volatile unsigned short *)0x04000130))
#define REG_SOUNDCNT_X     (*((volatile unsigned short *)0x04000084))

int main(void)
{
    volatile unsigned char *DISPCNT = (unsigned char *)0x04000000;

    DISPCNT[0] = 0x04; // Use video mode 4
    DISPCNT[1] = 0x04; // Enable BG2 (BG0 = 1, BG1 = 2, BG2 = 4, ...)

    volatile unsigned short *DMA3SAD = (unsigned short *)0x040000D4;
    volatile unsigned short *DMA3DAD = (unsigned short *)0x040000D8;
    volatile unsigned short *DMA3CNT = (unsigned short *)0x040000DC;
    volatile unsigned char *BG_Palette = (unsigned char *)0x050001FE;

    DMA3SAD[0] = 0x0000; // DMA 3 Source Address
    DMA3SAD[1] = 0x0A01;
    DMA3DAD[0] = 0x0000; // DMA 3 Destination Address
    DMA3DAD[1] = 0x0600;
    DMA3CNT[0] = 0x0F01; // DMA 3 Word Count (0x2581 = 240x160)
    DMA3CNT[1] = 0x0400; // DMA 3 Control
    BG_Palette[0] = 0xFF;
    BG_Palette[1] = 0xFF; // 256th Colour (White)

    volatile unsigned char *BG2PA = (unsigned char *)0x04000020;
    volatile unsigned char *BG2PD = (unsigned char *)0x04000026;

    BG2PA[0] = 0x89; // BG2 Scaling X-Axis
    BG2PA[1] = 0x00;
    BG2PD[0] = 0x80; // BG2 Scaling Y-Axis
    BG2PD[1] = 0x00;

    volatile unsigned short *SOUNDCNT = (unsigned short *)0x4000080;
    volatile unsigned char  *SOUND2CNT_L = (unsigned char *)0x04000068;
    volatile unsigned short *SOUND2CNT_H = (unsigned short *)0x0400006C;

    SOUNDCNT[2] = 0x0080;  // turn on sound circuit
    SOUNDCNT[0] = 0x2277;  // full volume, enable Sound 2 to left and right
    SOUNDCNT[1] = 0x0002;  // overall output ratio = full
    SOUND2CNT_L[0] = 0x80; // duty = 50%
    SOUND2CNT_L[1] = 0x00; // mute (maximum volume = 0xF0)

    unsigned short key_states = 0;
    unsigned short tempShort = 0;
    unsigned char  tempChar = 0;

    while(1)
    {
      // Skip past the rest of any current V-Blank, then skip past the V-Draw
      while(REG_DISPLAY_VCOUNT >= 160);
      while(REG_DISPLAY_VCOUNT <  160);

      if (REG_DMA3CNT_H == 0x0400)
      {
        // Get current key states (REG_KEY_INPUT stores the states inverted)
        key_states = REG_KEY_INPUT;

        // upper 4 bits store volume / mute, lower 11 bits store frequency
        tempShort = (*((volatile unsigned short *)(0x0A001000 + (key_states << 1)))); // 0x03FF
        tempChar  = (tempShort & 0xF000) >> 8;
        SOUND2CNT_L[1] = tempChar; // volume / mute
        if (tempChar == 0)
        {
            SOUND2CNT_H[0] = 0x4000; // frequency / reset
        }
        else if (REG_SOUNDCNT_X == 0x0080) // Sound 2 status off
        {
            SOUND2CNT_H[0] = 0x8000 | (tempShort & 0x07FF); // frequency / reset
        }

        if (key_states == 0x01FF) // BUTTON_L, Scaling ON
        {
            BG2PA[0] = 0x89; // BG2 Scaling X-Axis
            BG2PA[1] = 0x00;
            BG2PD[0] = 0x80; // BG2 Scaling Y-Axis
            BG2PD[1] = 0x00;
        }
        if (key_states == 0x02FF) // BUTTON_R, Scaling OFF
        {
            BG2PA[0] = 0x00; // BG2 Scaling X-Axis
            BG2PA[1] = 0x01;
            BG2PD[0] = 0x00; // BG2 Scaling Y-Axis
            BG2PD[1] = 0x01;
        }

        DMA3CNT[1] = 0x8400; // DMA 3 Control
      }
    }
    return 0;
}
