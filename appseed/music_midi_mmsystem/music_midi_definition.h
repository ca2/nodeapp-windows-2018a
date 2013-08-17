#pragma once


namespace music
{


   namespace midi
   {


      typedef struct tag_midiDoneData MIDIDONEDATA, * LPMIDIDONEDATA;
      class event_base;
      class event_v5;
      class events_v1;
      class track_base;
      class track;
      class lyric_track;
      class tracks;
      class file;
      class sequence;
      class sequence_thread;
      class CTrackArray;



      namespace player
      {

         class player;
         class command;



      } // namespace player


   } // namespace midi


} // namespace music




typedef uint32_t SMFRESULT;






//typedef struct tag_smf *PSMF;





//uint32_t FNLOCAL smfGetVDword(
//    byte *              hpbImage,
//  uint32_t               dwLeft,
//uint32_t *            pdw);

/*
** Useful macros when dealing with hi-lo format integers
*/
#define DWORDSWAP(dw) \
   ((((dw)>>24)&0x000000FFL)|\
   (((dw)>>8)&0x0000FF00L)|\
   (((dw)<<8)&0x00FF0000L)|\
   (((dw)<<24)&0xFF000000L))

#define WORDSWAP(w) \
   ((((w)>>8)&0x00FF)|\
   (((w)<<8)&0xFF00))

#define FOURCC_RMID     mmioFOURCC('R','M','I','D')
#define FOURCC_data     mmioFOURCC('d','a','t','a')
#define FOURCC_MThd     mmioFOURCC('M','T','h','d')
#define FOURCC_MTrk     mmioFOURCC('M','T','r','k')
#define FOURCC_XFIH     mmioFOURCC('X','F','I','H')
#define FOURCC_XFKM     mmioFOURCC('X','F','K','M')

typedef struct CLASS_DECL_VERIWELL_MULTIMEDIA tag_chunkhdr
{
   FOURCC  fourccType;
   uint32_t   dwLength;
}   CHUNKHDR,
   *PCHUNKHDR;

#pragma pack(1)   // override cl32 default packing, to match disk spfile->
typedef struct CLASS_DECL_VERIWELL_MULTIMEDIA tag_midifilehdr
{
   WORD    wFormat;
   WORD    wTracks;
   WORD    wDivision;
   uint32_t  GetQuarterNoteTicks()
   {
      return WORDSWAP(wDivision) & ~0x80000000;
   }
}   MIDIFILEHDR,
   *LPMIDIFILEHDR;
#pragma pack()

/* NOTE: This is arbitrary and only used if there is a tempo map but no
** entry at tick 0.
*/
typedef struct CLASS_DECL_VERIWELL_MULTIMEDIA tag_keyframe
{
   /*
   ** Meta events. All FF's indicates never seen.
   */
   BYTE        rbTempo[3];

   /*
   ** MIDI channel messages. FF indicates never seen.
   */
   BYTE        rbProgram[16];
   BYTE        rbControl[16*120];
}   KEYFRAME,
   FAR *PKEYFRAME;

#define KF_EMPTY ((BYTE)0xFF)




//#define MIDI_DEFAULT_TEMPO      (500000L)

//#define MIDI_MSG                ((BYTE)0x80)
//#define MIDI_NOTEOFF            ((BYTE)0x80)
//#define MIDI_NOTEON             ((BYTE)0x90)
//#define MIDI_POLYPRESSURE       ((BYTE)0xA0)
//#define MIDI_CONTROLCHANGE      ((BYTE)0xB0)
//#define MIDI_PROGRAMCHANGE      ((BYTE)0xC0)
//#define MIDI_CHANPRESSURE       ((BYTE)0xD0)
//#define MIDI_PITCHBEND          ((BYTE)0xE0)
//#define MIDI_META               ((BYTE)0xFF)
//#define MIDI_SYSEX              ((BYTE)0xF0)
//#define MIDI_SYSEXEND           ((BYTE)0xF7)

//#define MIDI_META_TRACKNAME     ((BYTE)0x03)
//#define MIDI_META_EOT           ((BYTE)0x2F)
//#define MIDI_META_TEMPO         ((BYTE)0x51)
//#define MIDI_META_TIMESIG       ((BYTE)0x58)
//#define MIDI_META_KEYSIG        ((BYTE)0x59)
//#define MIDI_META_SEQSPECIFIC   ((BYTE)0x7F)




//DECLARE_HANDLE(HSMF);

//typedef struct tag_smfopenstruct
//{
//    char *               pstrName;
//   LPTSTR               pstrName;
//    uint32_t               dwTimeDivision;
//    HSMF                hSmf;
//}   SMFOPENFILESTRUCT,
//    FAR *PSMFOPENFILESTRUCT;

typedef struct tag_smffileinfo
{
   uint32_t               dwTracks;
   uint32_t               dwFormat;
   uint32_t               dwTimeDivision;
   imedia::position               tkLength;
}   SMFFILEINFO,
   FAR *PSMFFILEINFO;



/* buffer described by LPMIDIHDR is in polymsg format, except that it
** can contain meta-events (which will be ignored during playback by
** the current system). This means we can use the pack functions, etc.
*/
#define PMSG_META       ((BYTE)0xC0)






namespace music
{

   namespace midi
   {



      CLASS_DECL_VERIWELL_MULTIMEDIA extern const uint32_t grbChanMsgLen[];
      CLASS_DECL_VERIWELL_MULTIMEDIA extern const uint32_t voiceText[];
      CLASS_DECL_VERIWELL_MULTIMEDIA extern const char pitchText[][5];
      CLASS_DECL_VERIWELL_MULTIMEDIA extern const uint32_t DefaultTempo;
      
      
      
      CLASS_DECL_VERIWELL_MULTIMEDIA ::multimedia::result TranslateSMFResult(::music::midi::e_file_result smfrc);
      CLASS_DECL_VERIWELL_MULTIMEDIA uint32_t GetVDWord(byte * & hpbMidiStream, uint32_t dwLeft, uint32_t &dwValue);
      CLASS_DECL_VERIWELL_MULTIMEDIA int32_t GetMessageLen(BYTE bEvent);
      CLASS_DECL_VERIWELL_MULTIMEDIA uint32_t GetStateMaxSize();
      CLASS_DECL_VERIWELL_MULTIMEDIA double GetNoteFrequency(double dA3Frequency,BYTE bNote);


   } // namespace midi


} // namespace music





