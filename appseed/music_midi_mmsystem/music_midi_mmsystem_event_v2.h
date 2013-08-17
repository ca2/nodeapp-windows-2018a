#pragma once


namespace music
{


   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA event_v2 :
         public ::music::midi::event_base  
      {
      public:


         imedia::position           m_tkDelta;           
         imedia::position           m_tkPosition;
         BYTE                       m_abEvent[3];        
         uint32_t                      m_cbParm;            
         byte *                     m_hpbParm; 
         uint32_t                      m_cbImage;
         uint32_t                      m_idxImage;
         uint32_t                      m_idxParam;
         sp(::music::midi::file)        m_pFile;
         ::music::midi::track *               m_pTrack;
         byte *                     m_hpbAllocation;
         uint32_t                      m_dwAllocation;
         bool                       m_bAutoAllocation;
         uint32_t                      m_dwAllocationAddUp;

         int32_t                        m_iFlags;


         event_v2(sp(::music::midi::file) pFile, ::music::midi::track * pTrack);
         event_v2(sp(::music::midi::file) pFile, ::music::midi::track * pTrack, event_v1 * pEvent);
         virtual ~event_v2();

         using ::music::midi::event_base::SetParam;
         virtual void clear();

         virtual ::music::midi::event_base & operator = (const ::music::midi::event_base & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual ::music::midi::event_base & operator = (const event_v5 & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual event_v2 & operator = (const event_v2 & eventSrc);

         bool allocate(uint32_t dwNewLength);
         bool allocate_add_up(uint32_t dwAddUp);
         void SetAutoAllocation(bool bValue = TRUE);

         bool IsAutoAllocated();

         void   AutoAllocate();
         SMFRESULT CreateMetaEvent(imedia::position tkDelta, BYTE bMetaType, byte * hpbParam, uint32_t cbLength);
         SMFRESULT CreateXFInfoHeaderEvent(imedia::position tkDelta, ::music::xf::info_header * pXfih);
         uint32_t SetVDWord(uint64_t ui);
         uint32_t SetVDWord(byte * hpbImage, uint32_t dw);
         inline imedia::position GetDelta() const;
         inline void SetDelta(imedia::position tkDelta);
         inline imedia::position get_position() const;
         inline void SetPosition(imedia::position tkPosition);

         inline BYTE GetFullType() const;
         inline void SetFullType(BYTE bValue);
         inline BYTE get_type() const;
         inline void set_type(BYTE bValue);
         inline BYTE GetTrack() const;
         inline void SetTrack(BYTE bValue);
         inline BYTE GetMetaType() const;
         inline void SetMetaType(BYTE bValue);
         inline BYTE GetChB1() const;
         inline void SetChB1(BYTE bValue);
         inline BYTE GetChB2() const;
         inline void SetChB2(BYTE bValue);
         inline BYTE GetNotePitch() const;
         inline void SetNotePitch(BYTE bValue);
         inline BYTE GetNoteVelocity() const;
         inline void SetNoteVelocity(BYTE bValue);
         void SetImage(byte * hpbImage);
         byte * GetImage() const;
         inline void SetImageSize(::primitive::memory_size cbSize);
         inline ::primitive::memory_size GetImageSize() const;
         inline void SetParam(void * pVoid, ::primitive::memory_size iSize);
         virtual VMSRESULT SetParam(::ca2::file & file, ::primitive::memory_size iLength = -1);
         inline byte * GetParam() const;
         inline ::primitive::memory_size GetParamSize() const;

         virtual int32_t GetFlags() const;
         virtual void SetFlags(int32_t iFlag);


      };


   } // namespace midi


} // namespace music





