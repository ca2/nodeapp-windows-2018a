#include "StdAfx.h"

DWORD g_dwPrepareSmallBell = 0;


void prepare_small_bell()
{
   g_dwPrepareSmallBell = ::GetTickCount();
   if(read_resource_as_file(dir::ca2("stage\\small_bell.mp3").c_str(),
      ::GetModuleHandleA("spalib.dll"), ID_MP3_SMALL_BELL, "MP3"))
   {
      if(mciSendString(("open \"" + dir::ca2("stage\\small_bell.mp3") + "\" type mpegvideo alias small_bell_mp3").c_str(), NULL, 0, NULL) == 0)
      {
         mciSendString("close small_bell_mp3", NULL, 0, NULL);
      }
   }
}

void defer_play_small_bell()
{
   if(::GetTickCount() - g_dwPrepareSmallBell > 10 * 1000)
   {
      play_small_bell();
   }
}

void play_small_bell()
{
   if(read_resource_as_file(dir::ca2("stage\\small_bell.mp3").c_str(),
      ::GetModuleHandleA("spalib.dll"), ID_MP3_SMALL_BELL, "MP3"))
   {
      if(mciSendString(("open \"" + dir::ca2("stage\\small_bell.mp3") + "\" type mpegvideo alias small_bell_mp3").c_str(), NULL, 0, NULL) == 0)
      {
         mciSendString("play small_bell_mp3", NULL, 0, NULL);
         Sleep(5000);
         mciSendString("close small_bell_mp3", NULL, 0, NULL);
      }
   }
}