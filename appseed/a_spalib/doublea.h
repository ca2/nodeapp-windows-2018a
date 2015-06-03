#pragma once

class doublea : public std::vector < double >
{
public:
   inline double average()
   {
      if(size() == 0)
         return 0.0;
      double d = 0.0;
      for(unsigned int ui = 0; ui < size(); ui++)
      {
         d += at(ui) / ((double) size());
      }
      return d;
   }
};
