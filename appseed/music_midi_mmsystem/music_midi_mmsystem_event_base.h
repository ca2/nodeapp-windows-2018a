#pragma once


class ::music::xf::info_header;


namespace music
{


   namespace midi
   {


      class file;
      class track;
      class event_v5;

      class CLASS_DECL_VERIWELL_MULTIMEDIA event_base
      {
      public:


         static uint32_t GetVDWord(byte * hpbImage, uint32_t dwLeft, uint32_t * pDw);
         static uint32_t SetVDWord(byte * hpbImage, uint32_t dwLeft,      uint32_t dw);

         virtual event_base & operator = (const event_base & eventSrc);
         virtual event_base & operator = (const event_v5 & eventSrc);

         virtual imedia::position GetDelta() const  = 0;
         virtual void SetDelta(imedia::position tkDelta) = 0;
         virtual imedia::position get_position() const = 0;
         virtual void SetPosition(imedia::position tkPosition) =  0;
         virtual BYTE GetFullType() const = 0;
         virtual void SetFullType(BYTE bValue) = 0;
         virtual BYTE get_type() const = 0;
         virtual void set_type(BYTE bValue) = 0;
         virtual BYTE GetTrack() const = 0;
         virtual void SetTrack(BYTE bValue) = 0;
         virtual BYTE GetMetaType() const = 0;
         virtual void SetMetaType(BYTE bValue) = 0;
         virtual BYTE GetChB1() const = 0;
         virtual void SetChB1(BYTE bValue) = 0;
         virtual BYTE GetChB2() const = 0;
         virtual void SetChB2(BYTE bValue) = 0;
         virtual BYTE GetNotePitch() const = 0;
         virtual void SetNotePitch(BYTE bValue) = 0;
         virtual BYTE GetNoteVelocity() const = 0;
         virtual void SetNoteVelocity(BYTE bValue) = 0;

         // Image
         virtual byte * GetImage() const = 0;
         virtual ::primitive::memory_size GetImageSize() const = 0;
         virtual void SetImage(byte * hpbImage) = 0;
         virtual void SetImageSize(::primitive::memory_size iSize) = 0;

         // Parameter
         virtual byte * GetParam() const = 0;
         virtual ::primitive::memory_size GetParamSize() const = 0;
         virtual void SetParam(void * pVoid, ::primitive::memory_size iSize) = 0;
         virtual void SetParam(primitive::memory & memstorage);
         virtual VMSRESULT SetParam(::ca2::file & file, ::primitive::memory_size iLength = -1) = 0;


         virtual int32_t GetPitchBendLevel() const;
         virtual int32_t GetFlags() const = 0;
         virtual void SetFlags(int32_t iFlag) = 0;
         virtual void clear() = 0;



      };   


   } // namespace midi


} // namespace music




