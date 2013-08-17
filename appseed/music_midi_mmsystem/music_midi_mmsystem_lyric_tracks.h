#pragma once


namespace music
{


   namespace midi
   {


      class lyric_tracks : public array<lyric_track, lyric_track &>
      {
      public:
         lyric_tracks();
         virtual ~lyric_tracks();

         VMSRESULT GetNextEventTkPosition(
            TICKS *					pTkPosition,
            TICKS                   tkMax);

         VMSRESULT GetNextEvent(
            CMidiEventV001 *			pEvent,
            TICKS                   tkMax,
            bool					bTkMaxInclusive);

         void SeekBegin();

      };


   } // namespace midi


} // namespace music





