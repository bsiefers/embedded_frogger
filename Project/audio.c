struct ckhd {

  uint32_t ckID;

  uint32_t cksize;

};

 

struct fmtck {

  uint16_t wFormatTag;     

  uint16_t nChannels;

  uint32_t nSamplesPerSec;

  uint32_t nAvgBytesPerSec;

  uint16_t nBlockAlign;

  uint16_t wBitsPerSample;

};

 

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {

  f_read(fid, hd, sizeof(struct ckhd), &ret);

  if (ret != sizeof(struct ckhd))

    exit(-1);

  if (ckID && (ckID != hd->ckID))

    exit(-1);

}

 

void playAudio(char* fname){

   FRESULT rc;

   FIL fid;/* File object */

   BYTE Buff[512];/* File read buffer */

   int ret;

 

   rc = f_open(&fid, "%s", fname);

   if (!rc) {

     struct ckhd hd;

     uint32_t  waveid;

     struct fmtck fck;

 

     readckhd(&fid, &hd, 'FFIR');

 

     f_read(&fid, &waveid, sizeof(waveid), &ret);

     if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))

       return -1;

 

     readckhd(&fid, &hd, ' tmf');

 

     f_read(&fid, &fck, sizeof(fck), &ret);

 

     // skip over extra info

 

     if (hd.cksize != 16) {

       printf("extra header info %d\n", hd.cksize - 16);      f_lseek(&fid, hd.cksize - 16);

     }

 

     // now skip all non-data chunks !

 

     while(1){

       readckhd(&fid, &hd, 0);

       if (hd.ckID == 'atad')

         break;

       f_lseek(&fid, hd.cksize);

     }

 

   f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);

   hd.cksize -= ret;

   audioplayerStart();

   }