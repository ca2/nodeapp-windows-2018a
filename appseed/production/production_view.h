#pragma once

namespace production
{

   class document;


   class CLASS_DECL_CA2_PRODUCTION view :
      public ::userbase::scroll_view
   {
   public:

      int32_t                     m_iStep;
      bool                    m_bLayout;

      int32_t                     m_iLineHeight;

      production *      m_pproduction;
      DWORD                   m_dwLastSnapshotUpdate;
      int32_t                     m_iW;
      int32_t                     m_iH;
      int32_t                     m_iWScreen;
      int32_t                     m_iHScreen;
      int32_t                     m_iArea;
      int32_t                     m_iItemHeight;
      int32_t                     m_iTaskOffset;
      // veriwell votagus vida
      int32_t                     m_iV; 
      int32_t                     m_iVH;
      int32_t                     m_iVW;
      // veriwell votagus ca2 out world stage
      int32_t                     m_iVs;
      int32_t                     m_iVsH;
      int32_t                     m_iVsW;
      visual::dib_sp             m_dibV;
      visual::dib_sp             m_dibVs;
      ::ca::brush_sp          m_brushBkActive;
      ::ca::brush_sp          m_brushBkInactive;
      simple_scroll_bar         m_scrollbarVert;
      simple_scroll_bar         m_scrollbarHorz;


	   view(::ca::application * papp);
	   virtual ~view();
   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      enum e_message
      {
         MessageOp = WM_USER + 1123,
      };

      enum EOp
      {
         OpUpdateCurrentArea,
      };


      virtual production * create_production();



      

      void GetAreaThumbRect(LPRECT lprect, int32_t iArea);
      index hit_test(point pt, ::user::control::e_element & eelement);



      void make_production();
      void production_loop(int32_t iLoopCount);



      virtual void _001OnDraw(::ca::graphics * pdc);

      virtual void _001OnTabClick(int32_t iTab);
      virtual void install_message_handling(::gen::message::dispatch * pinterface);
      virtual bool pre_create_window(CREATESTRUCT& cs);
      virtual void on_update(::view * pSender, LPARAM lHint, ::radix::object * pHint);

      document * get_document();

      DECL_GEN_SIGNAL(_001OnInitialUpdate)
      DECL_GEN_SIGNAL(_001OnLButtonDown)
      DECL_GEN_SIGNAL(_001OnLButtonUp)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnRButtonUp)
	   DECL_GEN_SIGNAL(_001OnDestroy)
	   DECL_GEN_SIGNAL(_001OnSize)
	   DECL_GEN_SIGNAL(_001OnCreate)
	   DECL_GEN_SIGNAL(_001OnContextMenu)
	   DECL_GEN_SIGNAL(_001OnSetCursor)
      DECL_GEN_SIGNAL(_001OnShowWindow)
      DECL_GEN_SIGNAL(_001OnUser)

   };

} // namespace production