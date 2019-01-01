# i n c l u d e   " v e c t o r . h "  
 # i n c l u d e   < i o s t r e a m >  
 # i n c l u d e   < W i n d o w s . h >  
 # i n c l u d e   < s t r i n g >  
 # i n c l u d e   < T l H e l p 3 2 . h >  
 # i n c l u d e   < v e c t o r >  
  
 u s i n g   n a m e s p a c e   s t d ;  
  
 D W O R D 6 4   W o r l d P T R ;  
 D W O R D 6 4   B l i p P T R ;  
 D W O R D 6 4   P l a y e r P T R ;  
 D W O R D 6 4   G l o b a l P T R ;  
  
 # p r a g m a   r e g i o n   u s e f u l l s t u f f  
  
 s t r u c t   M o d u l e   {  
         D W O R D 6 4   d w B a s e ,   d w S i z e ;  
 } ;  
  
 M o d u l e   T a r g e t M o d u l e ;  
 H A N D L E   T a r g e t P r o c e s s ;  
 D W O R D   T a r g e t I d ;  
  
 H A N D L E   G e t P r o c e s s ( c o n s t   w c h a r _ t   * p r o c e s s N a m e )   {  
         H A N D L E   h a n d l e   =   C r e a t e T o o l h e l p 3 2 S n a p s h o t ( T H 3 2 C S _ S N A P P R O C E S S ,   N U L L ) ;  
         P R O C E S S E N T R Y 3 2   e n t r y ;  
         e n t r y . d w S i z e   =   s i z e o f   e n t r y ;  
  
         d o  
                 i f   ( ! _ w c s i c m p ( e n t r y . s z E x e F i l e ,   p r o c e s s N a m e ) )   {  
                         T a r g e t I d   =   e n t r y . t h 3 2 P r o c e s s I D ;  
                         C l o s e H a n d l e ( h a n d l e ) ;  
                         T a r g e t P r o c e s s   =   O p e n P r o c e s s ( P R O C E S S _ A L L _ A C C E S S ,   f a l s e ,   T a r g e t I d ) ;  
                         r e t u r n   T a r g e t P r o c e s s ;  
                 }  
         w h i l e   ( P r o c e s s 3 2 N e x t ( h a n d l e ,   & e n t r y ) ) ;  
  
         r e t u r n   n u l l p t r ;  
 }  
  
 M o d u l e   G e t M o d u l e ( c o n s t   w c h a r _ t   * m o d u l e N a m e )   {  
         H A N D L E   h m o d u l e   =   C r e a t e T o o l h e l p 3 2 S n a p s h o t ( T H 3 2 C S _ S N A P M O D U L E ,   T a r g e t I d ) ;  
         M O D U L E E N T R Y 3 2   m E n t r y ;  
         m E n t r y . d w S i z e   =   s i z e o f   m E n t r y ;  
  
         d o   {  
                 i f   ( ! _ w c s i c m p ( m E n t r y . s z M o d u l e ,   m o d u l e N a m e ) )   {  
                         C l o s e H a n d l e ( h m o d u l e ) ;  
  
                         T a r g e t M o d u l e   =   { D W O R D 6 4 ( m E n t r y . h M o d u l e ) ,   m E n t r y . m o d B a s e S i z e } ;  
                         r e t u r n   T a r g e t M o d u l e ;  
                 }  
         }   w h i l e   ( M o d u l e 3 2 N e x t ( h m o d u l e ,   & m E n t r y ) ) ;  
  
         M o d u l e   m o d   =   { ( D W O R D 6 4 )   f a l s e ,   ( D W O R D 6 4 )   f a l s e } ;  
         r e t u r n   m o d ;  
 }  
  
 t e m p l a t e < t y p e n a m e   v a r >  
 B O O L   W r i t e M e m o r y ( D W O R D 6 4   A d d r e s s ,   v a r   V a l u e )   {  
         r e t u r n   W r i t e P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   ( L P V O I D )   A d d r e s s ,   & V a l u e ,   s i z e o f ( v a r ) ,   n u l l p t r ) ;  
 }  
  
 t e m p l a t e < t y p e n a m e   v a r >  
 v a r   R e a d M e m o r y ( D W O R D 6 4   A d d r e s s )   {  
         v a r   v a l u e ;  
         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   ( L P C V O I D )   A d d r e s s ,   & v a l u e ,   s i z e o f ( v a r ) ,   n u l l p t r ) ;  
         r e t u r n   v a l u e ;  
 }  
  
 B O O L   M e m o r y C o m p a r e ( c o n s t   B Y T E   * b D a t a ,   c o n s t   B Y T E   * b M a s k ,   c o n s t   c h a r   * s z M a s k )   {  
         f o r   ( i n t   k   =   0 ;   * s z M a s k ;   + + s z M a s k ,   + + b D a t a ,   + + b M a s k ,   + + k )   {  
                 i f   ( s t a t i c _ c a s t < c h a r > ( ( k   ^   1 )   ?   ( ~ ( * s z M a s k ) )   :   ( ( * s z M a s k )   ^   0 x D A ) )   = =   ' x '  
                         & &   * b D a t a   ! =   s t a t i c _ c a s t < u i n t 8 _ t > ( ( k   ^   1 )   ?   ( ( * b M a s k )   ^   0 x D A )   :   ( ~ ( * b M a s k ) ) ) )   {  
                         r e t u r n   f a l s e ;  
                 }  
         }  
         r e t u r n   * s z M a s k   = =   N U L L ;  
 }  
  
 u i n t p t r _ t   F i n d P o i n t e r ( u i n t p t r _ t   p t r ,   s t d : : v e c t o r < u n s i g n e d   i n t >   o f f s e t s )   {  
         u i n t p t r _ t   a d d r   =   p t r ;  
         f o r   ( u n s i g n e d   i n t   o f f s e t   :   o f f s e t s )   {  
                 R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( a d d r ) ,   & a d d r ,   s i z e o f   a d d r ,   n u l l p t r ) ;  
                 a d d r   + =   o f f s e t ;  
         }  
         r e t u r n   a d d r ;  
 }  
  
 u s i n g   s t d : : c o u t ;  
 u s i n g   s t d : : e n d l ;  
 u s i n g   s t d : : s t r i n g ;  
  
 D W O R D 6 4   F i n d S i g n a t u r e ( D W O R D 6 4   s t a r t ,   D W O R D 6 4   s i z e ,   c o n s t   c h a r   * s i g ,   c o n s t   c h a r   * m a s k )   {  
         a u t o   * d a t a   =   n e w   B Y T E [ s i z e ] ;  
         S I Z E _ T   b y t e s R e a d ;  
  
         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   ( L P V O I D )   s t a r t ,   d a t a ,   s i z e ,   & b y t e s R e a d ) ;  
  
         f o r   ( D W O R D 6 4   i   =   0 ;   i   <   s i z e ;   i + + )   {  
                 i f   ( M e m o r y C o m p a r e ( ( c o n s t   B Y T E   * )   ( d a t a   +   i ) ,   ( c o n s t   B Y T E   * )   s i g ,   m a s k ) )   {  
                         r e t u r n   s t a r t   +   i ;  
                 }  
         }  
         d e l e t e [ ]   d a t a ;  
         r e t u r n   N U L L ;  
 }  
  
 # p r a g m a   e n d r e g i o n  
  
 u i n t p t r _ t   W o r l d P t r B a s e A d d r ;  
 u i n t p t r _ t   B l i p P t r B a s e A d d r ;  
 u i n t p t r _ t   A f k P t r B a s e A d d r ;  
  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t s A F K   =   { 0 x 2 C 0 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t s W a n t e d L e v e l   =   { 0 x 8 ,   0 x 1 0 B 8 ,   0 x 8 1 8 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t s G o d M o d e   =   { 0 x 8 ,   0 x 2 8 0 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t s G o d M o d e t w o   =   { 0 x 8 ,   0 x 1 8 9 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t s R a g d o l l   =   { 0 x 8 ,   0 x 1 0 A 8 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t P l a y e r N a v   =   { 0 x 8 ,   0 x 3 0 ,   0 x 5 0 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t P l a y e r P o s   =   { 0 x 8 ,   0 x 9 0 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t C a r P o s   =   { 0 x 8 ,   0 x D 2 8 ,   0 x 3 0 ,   0 x 5 0 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t C a r N a v   =   { 0 x 8 ,   0 x D 2 8 ,   0 x 3 0 ,   0 x 9 0 } ;  
 s t d : : v e c t o r < u n s i g n e d   i n t >   O f f s e t s C a r G o d M o d e   =   { 0 x 8 ,   0 x D 2 8 ,   0 x 1 8 9 } ;  
  
 v o i d   t e l e p o r t ( v 3   v e c )   {  
         c o n s t   u i n t p t r _ t   p o s _ a d d r e s s _ c a r   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t C a r P o s ) ;  
         c o n s t   u i n t p t r _ t   n a v _ a d d r e s s _ c a r   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t C a r N a v ) ;  
         c o n s t   u i n t p t r _ t   p o s _ a d d r e s s   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t P l a y e r P o s ) ;  
         c o n s t   u i n t p t r _ t   n a v _ a d d r e s s   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t P l a y e r N a v ) ;  
         i f   ( G e t A s y n c K e y S t a t e ( V K _ S H I F T )   &   ( 1   < <   1 5 ) )   {  
                 v e c . z   =   - 2 2 5 . 0 f ;  
         }   e l s e   {  
                 v e c . z   + =   2 . 0 f ;  
         }  
         W r i t e M e m o r y < v 3 > ( n a v _ a d d r e s s _ c a r ,   v e c ) ;  
         W r i t e M e m o r y < v 3 > ( p o s _ a d d r e s s _ c a r ,   v e c ) ;  
         W r i t e M e m o r y < v 3 > ( n a v _ a d d r e s s ,   v e c ) ;  
         W r i t e M e m o r y < v 3 > ( p o s _ a d d r e s s ,   v e c ) ;  
 }  
  
 i n t   m a i n ( )   {  
         S e t C o n s o l e T i t l e ( L " N y a a " ) ;  
  
         L P C S T R   S i g n a t u r e W o r l d P T R   =   " \ x 9 2 \ x 7 4 \ x D F \ x D A \ x D A \ x D A \ x D A \ x 9 F \ x D A \ x D A \ x D A \ x D A \ x 9 2 \ x 5 1 \ x 9 2 \ x D 2 \ x 9 2 \ x 5 F \ x 1 3 \ x A E \ x D D " ;  
         L P C S T R   M a s k W o r l d P T R   =   " \ x 8 7 \ x A 2 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 " ;  
  
         L P C S T R   S i g n a t u r e B l i p P T R   =   " \ x 9 6 \ x 7 2 \ x D F \ x D A \ x D A \ x D A \ x D A \ x D 5 \ x 6 D \ x 1 B " ;  
         L P C S T R   M a s k B l i p P T R   =   " \ x 8 7 \ x A 2 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x 8 7 \ x 8 7 " ;  
  
         L P C S T R   S i g n a t u r e P l a y e r P T R   =   " \ x 9 2 \ x 7 4 \ x D 7 \ x D A \ x D A \ x D A \ x D A \ x 3 2 \ x D A \ x D A \ x D A \ x D A \ x 9 2 \ x 5 1 \ x 1 2 \ x 3 2 \ x D A \ x D A \ x D A \ x D A \ x 9 2 \ x 5 1 \ x 1 5 " ;  
         L P C S T R   M a s k P l a y e r P T R   =   " \ x 8 7 \ x A 2 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x 8 7 \ x 8 7 " ;  
  
         L P C S T R   S i g n a t u r e G l o b a l P T R   =   " \ x 9 6 \ x 7 2 \ x D F \ x D A \ x D A \ x D A \ x D A \ x 9 7 \ x 5 1 \ x D 2 \ x 9 7 \ x 5 F \ x 1 3 \ x A E \ x C B " ;  
         L P C S T R   M a s k G l o b a l P T R   =   " \ x 8 7 \ x A 2 \ x 8 7 \ x C 0 \ x C 0 \ x C 0 \ x C 0 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 \ x 8 7 " ;  
  
         i f   ( G e t P r o c e s s ( L " G T A 5 . e x e " ) )   {  
                 M o d u l e   m o d   =   G e t M o d u l e ( L " G T A 5 . e x e " ) ;  
  
                 D W O R D 6 4   T e m p W o r l d P T R   =   F i n d S i g n a t u r e ( m o d . d w B a s e ,   m o d . d w S i z e ,   S i g n a t u r e W o r l d P T R ,   M a s k W o r l d P T R ) ;  
                 W o r l d P T R   =   T e m p W o r l d P T R   +   R e a d M e m o r y < D W O R D > ( T e m p W o r l d P T R   +   3 )   +   7 ;  
  
                 D W O R D 6 4   T e m p B l i p P T R   =   F i n d S i g n a t u r e ( m o d . d w B a s e ,   m o d . d w S i z e ,   S i g n a t u r e B l i p P T R ,   M a s k B l i p P T R ) ;  
                 B l i p P T R   =   T e m p B l i p P T R   +   R e a d M e m o r y < D W O R D > ( T e m p B l i p P T R   +   3 )   +   7 ;  
  
                 D W O R D 6 4   T e m p P l a y e r P T R   =   F i n d S i g n a t u r e ( m o d . d w B a s e ,   m o d . d w S i z e ,   S i g n a t u r e P l a y e r P T R ,   M a s k P l a y e r P T R ) ;  
                 P l a y e r P T R   =   T e m p P l a y e r P T R   +   R e a d M e m o r y < D W O R D > ( T e m p P l a y e r P T R   +   3 )   +   7 ;  
  
                 D W O R D 6 4   T e m p G l o b a l P T R   =   F i n d S i g n a t u r e ( m o d . d w B a s e ,   m o d . d w S i z e ,   S i g n a t u r e G l o b a l P T R ,   M a s k G l o b a l P T R ) ;  
                 G l o b a l P T R   =   T e m p G l o b a l P T R   +   R e a d M e m o r y < D W O R D > ( T e m p G l o b a l P T R   +   3 )   +   7 ;  
         }  
  
         c o u t   < <   " "   < <   e n d l ;  
         c o u t   < <   " S o m e   f u n c t i o n s   m a y   n o t   w o r k   f o r   t h e   s t e a m   v e r s i o n . "   < <   e n d l ;  
         c o u t   < <   " "   < <   e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 2   t o   e n a b l e   N o   A F K ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 3   t o   r e m o v e   y o u r   w a n t e d   l e v e l ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 4   t o   e n a b l e   g o d   m o d e ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 5   t o   e n a b l e   n o   r a g d o l l ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 6   t o   t e l e p o r t   o n   t h e   w a y p o i n t ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 7   t o   t e l e p o r t   o n   t h e   o b j e c t i v e ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 8   t o   t e l e p o r t   o n   t h e   c a r g o ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " p r e s s   F 9   t o   t e l e p o r t   o n   t h e   t e r r o r b y t e ! "   < <   s t d : : e n d l ;  
         s t d : : c o u t   < <   " "   < <   s t d : : e n d l ;  
  
         W o r l d P t r B a s e A d d r   =   s t a t i c _ c a s t < u i n t p t r _ t > ( W o r l d P T R ) ;  
         B l i p P t r B a s e A d d r   =   s t a t i c _ c a s t < u i n t p t r _ t > ( B l i p P T R ) ;  
         A f k P t r B a s e A d d r   =   s t a t i c _ c a s t < u i n t p t r _ t > ( G l o b a l P T R   +   0 x 8 ) ;  
  
         / * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = * /  
         u i n t p t r _ t   A F K A d d r e s s   =   F i n d P o i n t e r ( A f k P t r B a s e A d d r ,   O f f s e t s A F K ) ;  
         u i n t p t r _ t   W a n t e d A d d r e s s   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t s W a n t e d L e v e l ) ;  
         u i n t p t r _ t   G o d m o d e A d d r e s s   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t s G o d M o d e ) ;  
         u i n t p t r _ t   G o d m o d e A d d r e s s t w o   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t s G o d M o d e t w o ) ;  
         u i n t p t r _ t   R a g d o l l A d d r e s s   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t s R a g d o l l ) ;  
         u i n t p t r _ t   C a r G o d M o d e A d d r e s s   =   F i n d P o i n t e r ( W o r l d P t r B a s e A d d r ,   O f f s e t s C a r G o d M o d e ) ;  
  
         c o u t   < <   " R a g d o l l   A d d r e s s   =   0 x "   < <   h e x   < <   R a g d o l l A d d r e s s   < <   e n d l ;  
         c o u t   < <   " G o d m o d e   A d d r e s s   =   0 x "   < <   h e x   < <   G o d m o d e A d d r e s s   < <   e n d l ;  
         c o u t   < <   " W a n t e d   A d d r e s s   =   0 x "   < <   h e x   < <   W a n t e d A d d r e s s   < <   e n d l ;  
         c o u t   < <   " A F K   A d d r e s s   =   0 x "   < <   h e x   < <   A F K A d d r e s s   < <   e n d l ;  
  
         s t d : : c o u t   < <   " "   < <   s t d : : e n d l ;  
         / * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = * /  
         b o o l   b N o A F K   =   f a l s e ;  
         b o o l   b N o W a n t e d   =   f a l s e ;  
         b o o l   b G o d   =   f a l s e ;  
         b o o l   b R a g d o l l   =   f a l s e ;  
 # p r a g m a   c l a n g   d i a g n o s t i c   p u s h  
 # p r a g m a   c l a n g   d i a g n o s t i c   i g n o r e d   " - W m i s s i n g - n o r e t u r n "  
         w h i l e   ( T R U E )   {  
                 S l e e p ( 1 ) ;  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 2 )   &   1 )   {  
                         b N o A F K   =   ! b N o A F K ;  
                         i f   ( b N o A F K )   {  
                                 s t d : : c o u t   < <   " A n t i   A F K :   o n "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( A F K A d d r e s s ,   1 2 3 4 5 6 7 8 9 0 ) ;  
                         }   e l s e   {  
                                 s t d : : c o u t   < <   " A n t i   A F K :   o f f "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( A F K A d d r e s s ,   1 0 ) ;  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 3 )   &   1 )   {  
                         b N o W a n t e d   =   ! b N o W a n t e d ;  
                         i f   ( b N o W a n t e d )   {  
                                 s t d : : c o u t   < <   " R e m o v e d   w a n t e d   l e v e l "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( W a n t e d A d d r e s s ,   0 ) ;  
                                 b N o W a n t e d   =   ! b N o W a n t e d ;  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 4 )   &   1 )   {  
                         b G o d   =   ! b G o d ;  
                         i f   ( b G o d )   {  
                                 s t d : : c o u t   < <   " G o d m o d e :   o n "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( G o d m o d e A d d r e s s t w o ,   t r u e ) ;  
                                 W r i t e M e m o r y ( G o d m o d e A d d r e s s ,   1 6 3 8 5 . 0 f ) ;  
                                 W r i t e M e m o r y ( C a r G o d M o d e A d d r e s s ,   t r u e ) ;  
                         }   e l s e   {  
                                 s t d : : c o u t   < <   " G o d m o d e :   o f f "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( G o d m o d e A d d r e s s t w o ,   0 ) ;  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 5 )   &   1 )   {  
                         b R a g d o l l   =   ! b R a g d o l l ;  
                         i f   ( b R a g d o l l )   {  
                                 s t d : : c o u t   < <   " N o   R a g d o l l :   o n "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( R a g d o l l A d d r e s s ,   t r u e ) ;  
                         }   e l s e   {  
                                 s t d : : c o u t   < <   " N o   R a g d o l l :   o f f "   < <   s t d : : e n d l ;  
                                 W r i t e M e m o r y ( R a g d o l l A d d r e s s ,   f a l s e ) ;  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 6 )   &   1 )   {  
                         a u t o   v e c   =   v 3 ( 0 ,   0 ,   - 2 2 5 . 0 f ) ;  
                         f o r   ( i n t   i   =   0 ;   i   <   2 0 0 0 ;   i + + )   {  
                                 a u t o   a   =   B l i p P t r B a s e A d d r ;  
                                 a u t o   n   =   R e a d M e m o r y < u i n t p t r _ t > ( a   +   i   *   8 ) ;  
                                 a u t o   t y p e   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 0 ) ;  
                                 a u t o   c o l o r   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 8 ) ;  
                                 i f   ( n   >   0   & &   8   = =   t y p e   & &   8 4   = =   c o l o r )   {  
                                         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( n   +   0 x 1 0 ) ,   & v e c ,   8 ,   n u l l p t r ) ;  
                                         t e l e p o r t ( v e c ) ;  
                                         c o u t   < <   " T e l e p o r t e d   t o   w a y p o i n t "   < <   e n d l ;  
                                         b r e a k ;  
                                 }  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 7 )   &   1 )   {  
                         a u t o   v e c   =   v 3 ( 0 ,   0 ,   - 2 2 5 ) ;  
                         f o r   ( i n t   i   =   0 ;   i   <   2 0 0 0 ;   i + + )   {  
                                 a u t o   a   =   B l i p P t r B a s e A d d r ;  
                                 a u t o   n   =   R e a d M e m o r y < u i n t p t r _ t > ( a   +   i   *   8 ) ;  
                                 a u t o   t y p e   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 0 ) ;  
                                 a u t o   c o l o r   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 8 ) ;  
                                 i f   ( n   >   0   & &   t y p e   = =   1   & &   ( c o l o r   = =   5   | |   c o l o r   = =   6 0   | |   c o l o r   = =   6 6 ) )   {  
                                         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( n   +   0 x 1 0 ) ,   & v e c ,   s i z e o f   v e c ,   n u l l p t r ) ;  
                                         i f   ( v e c . x   ! =   0   & &   v e c . y   ! =   0 )   {  
                                                 t e l e p o r t ( v e c ) ;  
                                                 c o u t   < <   " T e l e p o r t e d   t o   o b j e c t i v e "   < <   e n d l ;  
                                                 b r e a k ;  
                                         }  
                                         s t d : : c o u t   < <   " F o u n d   B l i p :   "   < <   s t d : : d e c   < <   t y p e   < <   "   c o l o r   "   < <   c o l o r   < <   "   a t   x : "   < <   v e c . x   < <   "   y : "  
                                                             < <   v e c . y  
                                                             < <   s t d : : e n d l ;  
                                 }  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 8 )   &   1 )   {  
                         a u t o   v e c   =   v 3 ( 0 ,   0 ,   - 2 2 5 ) ;  
                         f o r   ( i n t   i   =   0 ;   i   <   2 0 0 0 ;   i + + )   {  
                                 a u t o   a   =   B l i p P t r B a s e A d d r ;  
                                 a u t o   n   =   R e a d M e m o r y < u i n t p t r _ t > ( a   +   i   *   8 ) ;  
                                 a u t o   t y p e   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 0 ) ;  
                                 i f   ( n   >   0   & &   t y p e   = =   4 7 8 )   {  
                                         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( n   +   0 x 1 0 ) ,   & v e c ,   s i z e o f   v e c ,   n u l l p t r ) ;  
                                         t e l e p o r t ( v e c ) ;  
                                         c o u t   < <   " T e l e p o r t e d   t o   c a r g o "   < <   e n d l ;  
                                         b r e a k ;  
                                 }  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 9 )   &   1 )   {  
                         a u t o   v e c   =   v 3 ( 0 ,   0 ,   - 2 2 5 ) ;  
                         f o r   ( i n t   i   =   0 ;   i   <   2 0 0 0 ;   i + + )   {  
                                 a u t o   a   =   B l i p P t r B a s e A d d r ;  
                                 a u t o   n   =   R e a d M e m o r y < u i n t p t r _ t > ( a   +   i   *   8 ) ;  
                                 a u t o   t y p e   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 0 ) ;  
                                 i f   ( n   >   0   & &   t y p e   = =   6 3 2 )   {  
                                         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( n   +   0 x 1 0 ) ,   & v e c ,   s i z e o f   v e c ,   n u l l p t r ) ;  
                                         t e l e p o r t ( v e c ) ;  
                                         c o u t   < <   " T e l e p o r t e d   t o   t e r r o r b y t e "   < <   e n d l ;  
                                         b r e a k ;  
                                 }  
                         }  
                 }  
                 i f   ( G e t A s y n c K e y S t a t e ( V K _ F 1 2 )   &   1 )   {  
                         f l o a t   X ,   Y ;  
                         f o r   ( i n t   i   =   0 ;   i   <   2 0 0 0 ;   i + + )   {  
                                 a u t o   a   =   B l i p P t r B a s e A d d r ;  
                                 a u t o   n   =   R e a d M e m o r y < u i n t p t r _ t > ( a   +   i   *   8 ) ;  
                                 a u t o   t y p e   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 0 ) ;  
                                 a u t o   c o l o r   =   R e a d M e m o r y < D W O R D > ( n   +   0 x 4 8 ) ;  
                                 i f   ( n   >   0 )   {  
                                         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( n   +   0 x 1 0 ) ,   & X ,   s i z e o f   X ,   n u l l p t r ) ;  
                                         R e a d P r o c e s s M e m o r y ( T a r g e t P r o c e s s ,   r e i n t e r p r e t _ c a s t < B Y T E   * > ( n   +   0 x 1 4 ) ,   & Y ,   s i z e o f   Y ,   n u l l p t r ) ;  
  
                                         s t d : : c o u t   < <   " F o u n d   B l i p :   "   < <   s t d : : d e c   < <   t y p e   < <   "   c o l o r   "   < <   c o l o r   < <   "   a t   x : "   < <   X   < <   "   y : "   < <   Y  
                                                             < <   s t d : : e n d l ;  
                                 }  
                         }  
                 }  
         }  
 # p r a g m a   c l a n g   d i a g n o s t i c   p o p  
 }  
 