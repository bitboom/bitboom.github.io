---
layout: post
title:  Standardization of time
tags: [time] 
---

# 표준화 시간
인증서의 시간 비교와 같이 시차가 없는 표준화 시간을 사용해야 할 때가 있다.
이 때 사용하는 것이 협정 세계시 이다.

## [그리니치 평균시 (GMT)][1]
> 그리니치 평균시(Greenwich Mean Time, GMT)는 런던을 기점으로 하고,  
> 웰링턴에 종점으로 설정되는 협정 세계시의 빠른 시간대이다.


## [협정 세계시 (UTC)][2]
> 협정 세계시(Coordinated Universal Time)  
> 또는 UTC는 **1972년 1월 1일**부터 시행된 국제 표준시이다.  
> UTC는 국제원자시와 윤초 보정을 기반으로 표준화되었다.
> UTC는 그리니치 평균시(GMT)로 불리기도 하는데,  
> UTC와 GMT는 초의 소숫점 단위에서만 차이가 나기 때문에   
> 일상에서는 혼용되어 사용된다. 기술적인 표기에서는 UTC가 사용된다.  

## [유닉스 시간 (Unix time)][3]
> 유닉스 시간(영어: Unix time)은 시각을 나타내는 방식이다.  
> POSIX 시간이나 Epoch 시간이라고 부르기도 한다.  
> **1970년 1월 1일 00:00:00 협정 세계시(UTC)** 부터의 경과 시간을  
> 초로 환산하여 정수로 나타낸 것이다.

## [UTC vs Unix time][4]
> The precise definition of Unix time as an encoding of UTC  
> is only uncontroversial when applied to the present form of UTC.  
> Fortunately, the fact that the Unix epoch predates the start of  
> this form of UTC does not affect its use in this era: the number   
> of days from 1 January 1970 (the Unix epoch) to 1 January 1972  
> (the start of UTC) is not in question, and the number of days is    
> all that is significant to Unix time.  

# [C-example][5]

```
/* time example */
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

int main ()
{
  time_t timer;
  struct tm y2k = {0};
  double seconds;

  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));

  printf ("%.f seconds since January 1, 2000 in the current timezone", seconds);

  return 0;
}

``` 

  
[1]: <https://ko.wikipedia.org/wiki/%EA%B7%B8%EB%A6%AC%EB%8B%88%EC%B9%98_%ED%8F%89%EA%B7%A0%EC%8B%9C>
[2]: <https://ko.wikipedia.org/wiki/%ED%98%91%EC%A0%95_%EC%84%B8%EA%B3%84%EC%8B%9C>
[3]: <https://ko.wikipedia.org/wiki/%EC%9C%A0%EB%8B%89%EC%8A%A4_%EC%8B%9C%EA%B0%84>
[4]: <https://en.wikipedia.org/wiki/Unix_time#UTC_basis>
[5]: <http://www.cplusplus.com/reference/ctime/time/?kw=time>
