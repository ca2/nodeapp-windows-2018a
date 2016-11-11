

namespace draw2d_gdiplus
{

#define new AURA_NEW

   factory_exchange::factory_exchange(::aura::application * papp) :
      object(papp)
   {

      System.factory().cloneable_large < dib                   >  (System.type_info < ::draw2d::dib                  > ());
      System.factory().cloneable_large < graphics              >  (System.type_info < ::draw2d::graphics             > ());
      System.factory().cloneable_large < bitmap                >  (System.type_info < ::draw2d::bitmap               > ());
      System.factory().cloneable_large < pen                   >  (System.type_info < ::draw2d::pen                  > ());
      System.factory().cloneable_large < brush                 >  (System.type_info < ::draw2d::brush                > ());
      System.factory().cloneable_large < palette               >  (System.type_info < ::draw2d::palette              > ());
      System.factory().cloneable_large < region                >  (System.type_info < ::draw2d::region               > ());
      System.factory().cloneable_large < font                  >  (System.type_info < ::draw2d::font                 > ());
      System.factory().cloneable_large < path                  >  (System.type_info < ::draw2d::path                 > ());
      System.factory().cloneable_large < printer               >  (System.type_info < ::aura::printer                > ());
      System.factory().cloneable_large < window_gdi            >  (System.type_info < window_graphics                > ());

#undef new

      Gdiplus::Region * pregion = new Gdiplus::Region();

      output_debug_string("teshtchee!!");

   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace draw2d_gdiplus


#ifdef CUBE
void draw2d_factory_exchange(::aura::application * papp)
{
   draw2d_gdiplus::factory_exchange factoryexchange(papp);
}
#else
void ca2_factory_exchange(::aura::application * papp)
{
   draw2d_gdiplus::factory_exchange factoryexchange(papp);
}
#endif


