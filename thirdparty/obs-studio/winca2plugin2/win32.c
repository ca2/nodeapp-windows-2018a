#pragma once
#include <obs-module.h>
#define PSAPI_VERSION 1
#define WIN32_LEAN_AND_MEAN
#include <obs.h>
#include <util/dstr.h>
#include <stdlib.h>
#include <util/dstr.h>
#include <stdbool.h>
#include <windows.h>
#include <psapi.h>
#include <util/windows/win-version.h>
#include <util/platform.h>

#include <util/threading.h>


#pragma once

#include <stdint.h>
#include <inttypes.h>

#if defined(_UNICODE)
#define _T(x) L ##x
#else
#define _T(x) x
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
   CHAR szName[] = "Local\\ca2screen-%" PRIu64;
   CHAR szNameMutex[] = "Local\\ca2screenmutex-%" PRIu64;
#else
   char szName[] = "$HOME/.ca2/ca2screen-%" PRIu64;
   char szNameMutex[] = "$HOME/.ca2/ca2screenmutex-%" PRIu64;
#endif

   /* this is a workaround to A/Vs going crazy whenever certain functions (such as
   * OpenProcess) are used */
   extern void *get_obfuscated_func(HMODULE module, const char *str, uint64_t val);

#ifdef __cplusplus
}
#endif

#define NUM_TEXTURES 2

struct dc_capture {
   
   int          cur_tex;
   gs_texture_t *textures[NUM_TEXTURES];
   bool         textures_written[NUM_TEXTURES];
   int          x, y;
   uint32_t     width;
   uint32_t     height;
   int          num_textures;

   bool         compatibility;
   HDC          hdc;
   HBITMAP      bmp, old_bmp;
   BYTE         *bits;

   bool         capture_cursor;
   bool         cursor_captured;
   CURSORINFO   ci;

   bool         valid;
   COLORREF *   pcolorref;

};

extern void dc_capture_init(struct window_capture * wc, struct dc_capture *capture, int x, int y,
   uint32_t width, uint32_t height, bool cursor,
   bool compatibility);
extern void dc_capture_free(struct dc_capture *capture);

extern void dc_capture_capture(struct window_capture * wc, struct dc_capture *capture, HWND window);
extern void dc_capture_render(struct dc_capture *capture, gs_effect_t *effect);

#pragma once

#include <util/dstr.h>

enum window_priority {
   WINDOW_PRIORITY_CLASS,
   WINDOW_PRIORITY_TITLE,
   WINDOW_PRIORITY_EXE,
};

enum window_search_mode {
   INCLUDE_MINIMIZED,
   EXCLUDE_MINIMIZED
};

extern bool get_window_exe(struct dstr *name, HWND window);
extern void get_window_title(struct dstr *name, HWND hwnd);
extern void get_window_class(struct dstr *class, HWND hwnd);

typedef bool(*add_window_cb)(const char *title, const char *class,
   const char *exe);

extern void fill_window_list(obs_property_t *p, enum window_search_mode mode,
   add_window_cb callback);

extern void build_window_strings(const char *str,
   char **class,
   char **title,
   char **exe);

extern HWND find_window(enum window_search_mode mode,
   enum window_priority priority,
   const char *class,
   const char *title,
   const char *exe);





#define TEXT_WINDOW_CAPTURE obs_module_text("ca2Plugin")
#define TEXT_WINDOW         obs_module_text("ca2Plugin.Window")
#define TEXT_CAPTURE_CURSOR obs_module_text("CaptureCursor")
#define TEXT_COMPATIBILITY  obs_module_text("Compatibility")
#define TEXT_REFRESH_LIST  obs_module_text("RefreshList")

struct window_capture {
   obs_source_t         *source;
   obs_property_t *plist;

   char                 *title;
   char                 *class;
   char                 *executable;
   enum window_priority priority;
   bool                 cursor;
   bool                 compatibility;
   bool                 use_wildcards; /* TODO */

   struct dc_capture    capture;

   float                resize_timer;

   HWND                 window;
   RECT                 last_rect;
   pthread_mutex_t      mutex;
   
   HANDLE               m_hmutex;
   HWND                 m_hwnd;
   HANDLE               m_hMapFile;
   LPCTSTR              m_pBuf;


};

static obs_source_t *get_transition(struct window_capture *ss)
{
   obs_source_t *tr;

   pthread_mutex_lock(&ss->mutex);
   tr = ss->source;
   obs_source_addref(tr);
   pthread_mutex_unlock(&ss->mutex);

   return tr;
}


static void update_settings(struct window_capture *wc, obs_data_t *s)
{
   const char *window = obs_data_get_string(s, "window");
   int        priority = (int)obs_data_get_int(s, "priority");

   bfree(wc->title);
   bfree(wc->class);
   bfree(wc->executable);

   build_window_strings(window, &wc->class, &wc->title, &wc->executable);

   wc->priority = (enum window_priority)priority;
   wc->cursor = obs_data_get_bool(s, "cursor");
   wc->use_wildcards = obs_data_get_bool(s, "use_wildcards");
   //wc->compatibility = obs_data_get_bool(s, "compatibility");
   wc->compatibility = 1;
}

/* ------------------------------------------------------------------------- */

static const char *wc_getname(void *unused)
{
   UNUSED_PARAMETER(unused);
   return TEXT_WINDOW_CAPTURE;
}







static void *wc_create(obs_data_t *settings, obs_source_t *source)
{
   struct window_capture *wc = bzalloc(sizeof(struct window_capture));
   wc->source = source;

   pthread_mutex_init_value(&wc->mutex);
   if (pthread_mutex_init(&wc->mutex, NULL) != 0)
      return NULL;

   update_settings(wc, settings);
   return wc;
}

static void wc_destroy(void *data)
{
   struct window_capture *wc = data;

   if (wc)
   {

      obs_enter_graphics();
      dc_capture_free(&wc->capture);
      obs_leave_graphics();

      bfree(wc->title);
      bfree(wc->class);
      bfree(wc->executable);

      if (wc->m_hmutex != NULL)
      {

         if (WaitForSingleObject(wc->m_hmutex, INFINITE) == WAIT_OBJECT_0)
         {

            if (wc->m_pBuf != NULL)
            {

               UnmapViewOfFile(wc->m_pBuf);

               wc->m_pBuf = NULL;

            }

            if (wc->m_hMapFile != NULL)
            {

               CloseHandle(wc->m_hMapFile);

               wc->m_hMapFile = NULL;

            }

            ReleaseMutex(wc->m_hmutex);

         }

         CloseHandle(wc->m_hmutex);

      }


      bfree(wc);

   }
}

static void wc_update(void *data, obs_data_t *settings)
{
   struct window_capture *wc = data;
   update_settings(wc, settings);

   /* forces a reset */
   wc->window = NULL;
}

static uint32_t wc_width(void *data)
{
   struct window_capture *wc = data;
   return wc->capture.width;
}

static uint32_t wc_height(void *data)
{
   struct window_capture *wc = data;
   return wc->capture.height;
}


static void wc_defaults(obs_data_t *defaults)
{
   
   obs_data_set_default_bool(defaults, "cursor", true);
   
   obs_data_set_default_bool(defaults, "compatibility", false);

}


bool RefreshListClicked(obs_properties_t * props, obs_property_t *p, void *data)
{

   struct window_capture *wc = data;

   //if (wc->capture.valid)
   //{
   //   wc->resize_timer = 0.0f;
   //   dc_capture_free(&wc->capture);
   //   RECT rect;
   //   GetClientRect(wc->window, &rect);
   //   dc_capture_init(wc, &wc->capture, 0, 0, rect.right, rect.bottom,
   //      wc->cursor, wc->compatibility);
   //}


   fill_window_list(wc->plist, EXCLUDE_MINIMIZED, NULL);

   return true;

}


static obs_properties_t *wc_properties(void *data)
{

   struct window_capture *wc = data;
   

   obs_properties_t *ppts = obs_properties_create();

   wc->plist = obs_properties_add_list(ppts, "window", TEXT_WINDOW,
      OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
   fill_window_list(wc->plist, EXCLUDE_MINIMIZED, NULL);


   obs_properties_add_button(ppts, "refresh_list", TEXT_REFRESH_LIST, &RefreshListClicked);

   //p = obs_properties_add_list(ppts, "priority", TEXT_MATCH_PRIORITY,
   //   OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
   //obs_property_list_add_int(p, TEXT_MATCH_TITLE, WINDOW_PRIORITY_TITLE);
   //obs_property_list_add_int(p, TEXT_MATCH_CLASS, WINDOW_PRIORITY_CLASS);
   //obs_property_list_add_int(p, TEXT_MATCH_EXE, WINDOW_PRIORITY_EXE);

   //obs_properties_add_bool(ppts, "cursor", TEXT_CAPTURE_CURSOR);

   //obs_properties_add_bool(ppts, "compatibility", TEXT_COMPATIBILITY);

   return ppts;
}

#define RESIZE_CHECK_TIME 0.2f

static void wc_tick(void *data, float seconds)
{
   struct window_capture *wc = data;
   RECT rect;
   bool reset_capture = false;

   if (!obs_source_showing(wc->source))
      return;

   if (!wc->window || !IsWindow(wc->window)) {
      if (!wc->title && !wc->class)
         return;

      wc->window = find_window(EXCLUDE_MINIMIZED, wc->priority,
         wc->class, wc->title, wc->executable);
      if (!wc->window) {
         if (wc->capture.valid)
            dc_capture_free(&wc->capture);
         return;
      }

      reset_capture = true;

   }
   else if (IsIconic(wc->window)) {
      return;
   }

   obs_enter_graphics();

   GetClientRect(wc->window, &rect);

   if (!reset_capture) {
      wc->resize_timer += seconds;

      if (wc->resize_timer >= RESIZE_CHECK_TIME) {
         if (rect.bottom != wc->last_rect.bottom ||
            rect.right != wc->last_rect.right)
            reset_capture = true;

         wc->resize_timer = 0.0f;
      }
   }

   if (reset_capture) {
      wc->resize_timer = 0.0f;
      wc->last_rect = rect;
      dc_capture_free(&wc->capture);
      dc_capture_init(wc, &wc->capture, 0, 0, rect.right, rect.bottom,
         wc->cursor, wc->compatibility);
   }

   dc_capture_capture(wc, &wc->capture, wc->window);
   obs_leave_graphics();
}

static void wc_render(void *data, gs_effect_t *effect)
{
   struct window_capture *wc = data;
   dc_capture_render(&wc->capture, obs_get_base_effect(OBS_EFFECT_PREMULTIPLIED_ALPHA));

   UNUSED_PARAMETER(effect);
}


#include "audio_render.c"

struct obs_source_info window_capture_info = {
   .id = "ca2plugin",
   .type = OBS_SOURCE_TYPE_INPUT,
   .output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_COMPOSITE,
   .get_name = wc_getname,
   .create = wc_create,
   .destroy = wc_destroy,
   .update = wc_update,
   .video_render = wc_render,
   .video_tick = wc_tick,
   .audio_render = ss_audio_render,


   .get_width = wc_width,
   .get_height = wc_height,
   .get_defaults = wc_defaults,
   .get_properties = wc_properties
};




static inline void init_textures(struct dc_capture *capture)
{
   for (int i = 0; i < capture->num_textures; i++) {
      if (capture->compatibility)
         capture->textures[i] = gs_texture_create(
            capture->width, capture->height,
            GS_BGRA, 1, NULL, GS_DYNAMIC);
      else
         capture->textures[i] = gs_texture_create_gdi(
            capture->width, capture->height);

      if (!capture->textures[i]) {
         blog(LOG_WARNING, "[dc_capture_init] Failed to "
            "create textures");
         return;
      }
   }

   capture->valid = true;
}

void dc_capture_init(struct window_capture * wc, struct dc_capture *capture, int x, int y,
   uint32_t width, uint32_t height, bool cursor,
   bool compatibility)
{
   memset(capture, 0, sizeof(struct dc_capture));

   capture->x = x;
   capture->y = y;
   capture->width = width;
   capture->height = height;
   capture->capture_cursor = cursor;

   obs_enter_graphics();

   if (!gs_gdi_texture_available())
      compatibility = true;

   capture->compatibility = compatibility;
   capture->num_textures = compatibility ? 1 : 2;

   init_textures(capture);

   obs_leave_graphics();

   if (!capture->valid)
      return;

   if (compatibility) {
      BITMAPINFO bi = { 0 };
      BITMAPINFOHEADER *bih = &bi.bmiHeader;
      bih->biSize = sizeof(BITMAPINFOHEADER);
      bih->biBitCount = 32;
      bih->biWidth = width;
      bih->biHeight = height;
      bih->biPlanes = 1;

      capture->hdc = CreateCompatibleDC(NULL);
      capture->bmp = CreateDIBSection(capture->hdc, &bi,
         DIB_RGB_COLORS, (void**)&capture->bits,
         NULL, 0);
      capture->pcolorref = (COLORREF *)malloc(width * height * 4);
      capture->old_bmp = SelectObject(capture->hdc, capture->bmp);
   }

   if (wc->m_hmutex == NULL || wc->m_hwnd != wc->window)
   {

      wc->m_hwnd = wc->window;

      if (wc->m_hmutex != NULL)
      {

         if (WaitForSingleObject(wc->m_hmutex, INFINITE) == WAIT_OBJECT_0)
         {

            if (wc->m_pBuf != NULL)
            {

               UnmapViewOfFile(wc->m_pBuf);

               wc->m_pBuf = NULL;

            }

            if (wc->m_hMapFile != NULL)
            {

               CloseHandle(wc->m_hMapFile);

               wc->m_hMapFile = NULL;

            }

            ReleaseMutex(wc->m_hmutex);

         }

         CloseHandle(wc->m_hmutex);

      }

      char szNameMutex2[2048];

      sprintf(szNameMutex2, szNameMutex, (INT_PTR)wc->m_hwnd);

      wc->m_hmutex = CreateMutexA(NULL, FALSE, szNameMutex2);

      if (WaitForSingleObject(wc->m_hmutex, INFINITE) == WAIT_OBJECT_0)
      {

         char szName2[2048];

         sprintf(szName2, szName, (INT_PTR)wc->m_hwnd);

         wc->m_hMapFile = CreateFileMappingA(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            8192 * 4096 * 4,                // maximum object size (low-order DWORD)
            szName2);                 // name of mapping object

         if (wc->m_hMapFile == NULL)
         {

            ReleaseMutex(wc->m_hmutex);

            CloseHandle(wc->m_hmutex);

         }
         else
         {

            wc->m_pBuf = (LPTSTR)MapViewOfFile(wc->m_hMapFile,   // handle to map object
               FILE_MAP_ALL_ACCESS, // read/write permission
               0,
               0,
               8192 * 4096 * 4);

            if (wc->m_pBuf == NULL)
            {

               CloseHandle(wc->m_hMapFile);

               ReleaseMutex(wc->m_hmutex);

               CloseHandle(wc->m_hmutex);

            }

         }

         ReleaseMutex(wc->m_hmutex);

      }

   }




}

void dc_capture_free(struct dc_capture *capture)
{
   if (capture->hdc) {
      free(capture->pcolorref);
      SelectObject(capture->hdc, capture->old_bmp);
      DeleteDC(capture->hdc);
      DeleteObject(capture->bmp);
   }

   obs_enter_graphics();

   for (int i = 0; i < capture->num_textures; i++)
      gs_texture_destroy(capture->textures[i]);

   obs_leave_graphics();

   memset(capture, 0, sizeof(struct dc_capture));

   
}

static void draw_cursor(struct dc_capture *capture, HDC hdc, HWND window)
{
   HICON      icon;
   ICONINFO   ii;
   CURSORINFO *ci = &capture->ci;
   POINT      win_pos = { capture->x, capture->y };

   if (!(capture->ci.flags & CURSOR_SHOWING))
      return;

   icon = CopyIcon(capture->ci.hCursor);
   if (!icon)
      return;

   if (GetIconInfo(icon, &ii)) {
      POINT pos;

      if (window)
         ClientToScreen(window, &win_pos);

      pos.x = ci->ptScreenPos.x - (int)ii.xHotspot - win_pos.x;
      pos.y = ci->ptScreenPos.y - (int)ii.yHotspot - win_pos.y;

      DrawIcon(hdc, pos.x, pos.y, icon);

      DeleteObject(ii.hbmColor);
      DeleteObject(ii.hbmMask);
   }

   DestroyIcon(icon);
}

static inline HDC dc_capture_get_dc(struct dc_capture *capture)
{
   if (!capture->valid)
      return NULL;

   if (capture->compatibility)
      return capture->hdc;
   else
      return gs_texture_get_dc(capture->textures[capture->cur_tex]);
}

static inline void dc_capture_release_dc(struct dc_capture *capture)
{
   if (capture->compatibility) {
      gs_texture_set_image(capture->textures[capture->cur_tex],
         capture->bits, capture->width * 4, false);
   }
   else {
      gs_texture_release_dc(capture->textures[capture->cur_tex]);
   }
}

void dc_capture_capture(struct window_capture * wc, struct dc_capture *capture, HWND window)
{
   HDC hdc;

   if (capture->capture_cursor) {
      memset(&capture->ci, 0, sizeof(CURSORINFO));
      capture->ci.cbSize = sizeof(CURSORINFO);
      capture->cursor_captured = GetCursorInfo(&capture->ci);
   }

   if (++capture->cur_tex == capture->num_textures)
      capture->cur_tex = 0;

   hdc = dc_capture_get_dc(capture);
   if (!hdc) {
      blog(LOG_WARNING, "[capture_screen] Failed to get "
         "texture DC");
      return;
   }
   HBITMAP bmp = capture->bmp;
   //hdc_target = GetDC(window);




   if (wc->m_hmutex != NULL)
   {

      WaitForSingleObject(wc->m_hmutex, INFINITE);

      if(wc->m_pBuf != NULL)
      {

         BITMAPINFO m_bitmapinfo;

         ZeroMemory(&m_bitmapinfo, sizeof(m_bitmapinfo));

         int64_t * p = (int64_t *)wc->m_pBuf;

         int64_t cxParam = *p++;
         int64_t cyParam = *p++;
         int64_t m_iScan = *p++;

         m_bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
         m_bitmapinfo.bmiHeader.biWidth = (LONG)cxParam;
         m_bitmapinfo.bmiHeader.biHeight = (LONG)-cyParam;
         m_bitmapinfo.bmiHeader.biPlanes = 1;
         m_bitmapinfo.bmiHeader.biBitCount = 32;
         m_bitmapinfo.bmiHeader.biCompression = BI_RGB;
         m_bitmapinfo.bmiHeader.biSizeImage = (LONG)(cxParam * sizeof(COLORREF) * cyParam);

         //COLORREF * m_pcolorref = NULL;
         //HDC hdc_target = CreateCompatibleDC(NULL);
         //HBITMAP hbmp = CreateDIBSection(NULL, &m_bitmapinfo, DIB_RGB_COLORS, (void **)&m_pcolorref, NULL, 0);

         ////         memcpy(m_pcolorref, p, m_bitmapinfo.bmiHeader.biSizeImage);
         //int64_t area = cxParam * cyParam;

         //memcpy(capture->pcolorref, p, area * sizeof(COLORREF));

         //unsigned char * p1 = (unsigned char *)capture->pcolorref;


         //while (area > 0)
         //{

         //   p1[0] = p1[0] * p1[3] / 255;
         //   p1[1] = p1[1] * p1[3] / 255;
         //   p1[2] = p1[2] * p1[3] / 255;

         //   p1 += 4;
         //   area--;

         //}

         SetDIBits(hdc, bmp, 0, (UINT) cyParam, p, &m_bitmapinfo, DIB_RGB_COLORS);
         //Bit

         //SelectObject(hdc_target, hbmp);
         //BitBlt(hdc_target, 0, 0, capture->width, capture->height,
           // hdc, capture->x, capture->y, SRCCOPY);
         //BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
         //AlphaBlend(hdc_target, 0, 0, capture->width, capture->height,
         //   hdc_target, 0, 0, capture->width, capture->height,
         //   blendPixelFunction);



         // DeleteDC(hdc_target);

         //DeleteObject(hbmp);

      }

      ReleaseMutex(wc->m_hmutex);

   }


   //hdc_target = GetDCEx(window, NULL, DCX_WINDOW);
   //hdc_target = GetWindowLongPtr(window, GWLP_USERDATA);
   // Note: CAPTUREBLT flag is required to capture layered windows
   //http://forums.codeguru.com/member.php?89934-ovidiucucu&s=993e355d39eaa5c27a1f83c614cafa87
   //http://forums.codeguru.com/showthread.php?477515-Windows-SDK-GDI-How-to-capture-layered-windows
   //DWORD dwRop = SRCCOPY | CAPTUREBLT;
   //BOOL bRet = BitBlt(hdc, 0, 0, capture->width, capture->height, hdc_target, capture->x, capture->y, dwRop);
   //BitBlt(hdc, 0, 0, capture->width, capture->height,
     // hdc_target, capture->x, capture->y, dwRop);
   //BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
   //AlphaBlend(hdc, 0, 0, capture->width, capture->height,
   //   hdc_target, 0, 0, capture->width, capture->height,
   //   blendPixelFunction);

   //ReleaseDC(window, hdc_target);

   if (capture->cursor_captured)
      draw_cursor(capture, hdc, window);

   dc_capture_release_dc(capture);

   capture->textures_written[capture->cur_tex] = true;
}

static void draw_texture(struct dc_capture *capture, int id,
   gs_effect_t *effect)
{
   gs_texture_t   *texture = capture->textures[id];
   gs_technique_t *tech = gs_effect_get_technique(effect, "Draw");
   gs_eparam_t    *image = gs_effect_get_param_by_name(effect, "image");
   size_t      passes;

   gs_effect_set_texture(image, texture);

   passes = gs_technique_begin(tech);
   for (size_t i = 0; i < passes; i++) {
      if (gs_technique_begin_pass(tech, i)) {
         if (capture->compatibility)
         {
            gs_blend_state_push();
            //gs_blend_function_separate(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA, GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);
            gs_blend_function(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA);
            // gs_blend_function(GS_BLEND_SRCALPHA, GS_BLEND_SRCALPHA);
            //gs_blend_function_separate(GS_BLEND_SRCALPHA, GS_BLEND_ONE, GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA);
            gs_enable_blending(true);
            //gs_reset_blend_state();
            gs_draw_sprite(texture, GS_FLIP_V, 0, 0);
            gs_blend_state_pop();
         }
         else
            gs_draw_sprite(texture, 0, 0, 0);

         gs_technique_end_pass(tech);
      }
   }
   gs_technique_end(tech);
}

void dc_capture_render(struct dc_capture *capture, gs_effect_t *effect)
{
   int last_tex = (capture->cur_tex > 0) ?
      capture->cur_tex - 1 : capture->num_textures - 1;

   if (!capture->valid)
      return;

   if (capture->textures_written[last_tex])
      draw_texture(capture, last_tex, effect);
}



static inline void encode_dstr(struct dstr *str)
{
   dstr_replace(str, "#", "#22");
   dstr_replace(str, ":", "#3A");
}

static inline char *decode_str(const char *src)
{
   struct dstr str = { 0 };
   dstr_copy(&str, src);
   dstr_replace(&str, "#3A", ":");
   dstr_replace(&str, "#22", "#");
   return str.array;
}

extern void build_window_strings(const char *str,
   char **class,
   char **title,
   char **exe)
{
   char **strlist;

   *class = NULL;
   *title = NULL;
   *exe = NULL;

   if (!str) {
      return;
   }

   strlist = strlist_split(str, ':', true);

   if (strlist && strlist[0] && strlist[1] && strlist[2]) {
      *title = decode_str(strlist[0]);
      *class = decode_str(strlist[1]);
      *exe = decode_str(strlist[2]);
   }

   strlist_free(strlist);
}

static HMODULE kernel32(void)
{
   static HMODULE kernel32_handle = NULL;
   if (!kernel32_handle)
      kernel32_handle = GetModuleHandleA("kernel32");
   return kernel32_handle;
}

static inline HANDLE open_process(DWORD desired_access, bool inherit_handle,
   DWORD process_id)
{
   static HANDLE(WINAPI *open_process_proc)(DWORD, BOOL, DWORD) = NULL;
   if (!open_process_proc)
      open_process_proc = get_obfuscated_func(kernel32(),
         "B}caZyah`~q", 0x2D5BEBAF6DDULL);

   return open_process_proc(desired_access, inherit_handle, process_id);
}

bool get_window_exe(struct dstr *name, HWND window)
{
   wchar_t     wname[MAX_PATH];
   struct dstr temp = { 0 };
   bool        success = false;
   HANDLE      process = NULL;
   char        *slash;
   DWORD       id;

   GetWindowThreadProcessId(window, &id);
   if (id == GetCurrentProcessId())
      return false;

   process = open_process(PROCESS_QUERY_LIMITED_INFORMATION, false, id);
   if (!process)
      goto fail;

   if (!GetProcessImageFileNameW(process, wname, MAX_PATH))
      goto fail;

   dstr_from_wcs(&temp, wname);
   slash = strrchr(temp.array, '\\');
   if (!slash)
      goto fail;

   dstr_copy(name, slash + 1);
   success = true;

fail:
   if (!success)
      dstr_copy(name, "unknown");

   dstr_free(&temp);
   CloseHandle(process);
   return true;
}

void get_window_title(struct dstr *name, HWND hwnd)
{
   wchar_t *temp;
   int len;

   len = GetWindowTextLengthW(hwnd);
   if (!len)
      return;

   temp = malloc(sizeof(wchar_t) * (len + 1));
   if (GetWindowTextW(hwnd, temp, len + 1))
      dstr_from_wcs(name, temp);
   free(temp);
}

void get_window_class(struct dstr *class, HWND hwnd)
{
   wchar_t temp[256];

   temp[0] = 0;
   if (GetClassNameW(hwnd, temp, sizeof(temp) / sizeof(wchar_t)))
      dstr_from_wcs(class, temp);
}

static void add_window(obs_property_t *p, HWND hwnd, add_window_cb callback)
{
   struct dstr class = { 0 };
   struct dstr title = { 0 };
   struct dstr exe = { 0 };
   struct dstr encoded = { 0 };
   struct dstr desc = { 0 };

   if (!get_window_exe(&exe, hwnd))
      return;
   get_window_title(&title, hwnd);
   get_window_class(&class, hwnd);

   if (callback && !callback(title.array, class.array, exe.array)) {
      dstr_free(&title);
      dstr_free(&class);
      dstr_free(&exe);
      return;
   }

   dstr_printf(&desc, "[%s]: %s", exe.array, title.array);

   encode_dstr(&title);
   encode_dstr(&class);
   encode_dstr(&exe);

   dstr_cat_dstr(&encoded, &title);
   dstr_cat(&encoded, ":");
   dstr_cat_dstr(&encoded, &class);
   dstr_cat(&encoded, ":");
   dstr_cat_dstr(&encoded, &exe);

   obs_property_list_add_string(p, desc.array, encoded.array);

   dstr_free(&encoded);
   dstr_free(&desc);
   dstr_free(&class);
   dstr_free(&title);
   dstr_free(&exe);
}

static bool check_window_valid(HWND window, enum window_search_mode mode)
{
   DWORD styles, ex_styles;
   RECT  rect;

   if (!IsWindowVisible(window) ||
      (mode == EXCLUDE_MINIMIZED && IsIconic(window)))
      return false;

   GetClientRect(window, &rect);
   styles = (DWORD)GetWindowLongPtr(window, GWL_STYLE);
   ex_styles = (DWORD)GetWindowLongPtr(window, GWL_EXSTYLE);

   if (ex_styles & WS_EX_TOOLWINDOW)
      return false;
   if (styles & WS_CHILD)
      return false;
   if (mode == EXCLUDE_MINIMIZED && (rect.bottom == 0 || rect.right == 0))
      return false;

   return true;
}

static inline HWND next_window(HWND window, enum window_search_mode mode)
{
   while (true) {
      window = GetNextWindow(window, GW_HWNDNEXT);
      if (!window || check_window_valid(window, mode))
         break;
   }

   return window;
}

static inline HWND first_window(enum window_search_mode mode)
{
   HWND window = GetWindow(GetDesktopWindow(), GW_CHILD);
   if (!check_window_valid(window, mode))
      window = next_window(window, mode);
   return window;
}


static int ca2_window(HWND window)
{
   struct dstr cur_class = { 0 };
   struct dstr cur_title = { 0 };
   //struct dstr cur_exe = { 0 };
   int total = 0;

//   if (!get_window_exe(&cur_exe, window))
  //    return 0;
   get_window_title(&cur_title, window);
   get_window_class(&cur_class, window);




   if (dstr_cmpi(&cur_class, "ca2FrameOrView") == 0)
      total += 1;


   dstr_free(&cur_class);
   dstr_free(&cur_title);
   //dstr_free(&cur_exe);

   return total;
}


void fill_window_list(obs_property_t *p, enum window_search_mode mode,
   add_window_cb callback)
{
   HWND window = first_window(mode);

   obs_property_list_clear(p);

   while (window) {

      if (ca2_window(window))
      {
       
         add_window(p, window, callback);

      }
      window = next_window(window, mode);
   }
}

static int window_rating(HWND window,
   enum window_priority priority,
   const char *class,
   const char *title,
   const char *exe)
{
   struct dstr cur_class = { 0 };
   struct dstr cur_title = { 0 };
   struct dstr cur_exe = { 0 };
   int         class_val = 1;
   int         title_val = 1;
   int         exe_val = 0;
   int         total = 0;

   if (!get_window_exe(&cur_exe, window))
      return 0;
   get_window_title(&cur_title, window);
   get_window_class(&cur_class, window);



   if (priority == WINDOW_PRIORITY_CLASS)
      class_val += 3;
   else if (priority == WINDOW_PRIORITY_TITLE)
      title_val += 3;
   else
      exe_val += 3;

   if (dstr_cmpi(&cur_class, class) == 0)
      total += class_val;
   if (dstr_cmpi(&cur_title, title) == 0)
      total += title_val;
   if (dstr_cmpi(&cur_exe, exe) == 0)
      total += exe_val;

   dstr_free(&cur_class);
   dstr_free(&cur_title);
   dstr_free(&cur_exe);

   return total;
}

HWND find_window(enum window_search_mode mode,
   enum window_priority priority,
   const char *class,
   const char *title,
   const char *exe)
{
   HWND window = first_window(mode);
   HWND best_window = NULL;
   int  best_rating = 0;

   while (window) {
      if (ca2_window(window))
      {
         int rating = window_rating(window, priority, class, title, exe);
         if (rating > best_rating) {
            best_rating = rating;
            best_window = window;
         }

      }
      window = next_window(window, mode);
   }

   return best_window;
}




OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("ca2plugin", "en-US")

extern struct obs_source_info window_capture_info;


/* temporary, will eventually be erased once we figure out how to create both
* 32bit and 64bit versions of the helpers/hook */
#ifdef _WIN64
#define IS32BIT false
#else
#define IS32BIT true
#endif

#define USE_HOOK_ADDRESS_CACHE false

bool obs_module_load(void)
{
   struct win_version_info ver;
   bool win8_or_above = false;
   char *config_dir;

   config_dir = obs_module_config_path(NULL);
   if (config_dir) {
      os_mkdirs(config_dir);
      bfree(config_dir);
   }

   get_win_ver(&ver);

   win8_or_above = ver.major > 6 || (ver.major == 6 && ver.minor >= 2);

   obs_enter_graphics();

   
   obs_leave_graphics();

   obs_register_source(&window_capture_info);


   return true;
}



#define _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER
#pragma warning(disable : 4152) /* casting func ptr to void */
#endif

#include <windows.h>

#define LOWER_HALFBYTE(x) ((x) & 0xF)
#define UPPER_HALFBYTE(x) (((x) >> 4) & 0xF)

static void deobfuscate_str(char *str, uint64_t val)
{
   uint8_t *dec_val = (uint8_t*)&val;
   int i = 0;

   while (*str != 0) {
      int pos = i / 2;
      bool bottom = (i % 2) == 0;
      uint8_t *ch = (uint8_t*)str;
      uint8_t xor = bottom ?
         LOWER_HALFBYTE(dec_val[pos]) :
         UPPER_HALFBYTE(dec_val[pos]);

      *ch ^= xor;

      if (++i == sizeof(uint64_t) * 2)
         i = 0;

      str++;
   }
}

void *get_obfuscated_func(HMODULE module, const char *str, uint64_t val)
{
   char new_name[128];
   strcpy(new_name, str);
   deobfuscate_str(new_name, val);
   return GetProcAddress(module, new_name);
}
