/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * cinit.c
 * part of cLinux/cinit
 */

/* this code is stolen from gpm */

/* what's the english name for potenz ? */
int x_high_y(int base, int pot_y)
{
   int val = 1;
   
   if(pot_y == 0) val = 1;
   else if(pot_y  < 0) val = 0;     /* ugly hack ;) */
   else while(pot_y > 0) {
      val = val * base;
      pot_y--;
   }   
   return val;
}   
      
/* return characters needed to display int */
int cnt_digits(int number)
{
   /* 0-9 = 1        10^0 <-> (10^1)-1
    * 10 - 99 = 2    10^1 <-> (10^2)-1
    * 100 - 999 = 3  10^2 <-> (10^3)-1
    * 1000 - 9999 = 4 ...  */

   int ret = 0, num = 0;

   /* non negative, please */
   if(number < 0) number *= -1;
   else if(number == 0) ret = 1;
   else while(number > num) {
      ret++;
      num = (x_high_y(10,ret) - 1);
   }    
   
   return(ret); 
}
