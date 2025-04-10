
#ifndef _BIT_MATH_H
#define _BIT_MATH_H

#define SET_REG(REG_NAME)    ( REG_NAME =0xFFUL)
#define CLR_REG(REG_NAME)    ( REG_NAME =0x00UL)
#define TOG_REG(REG_NAME)    ( (REG_NAME)^=0xFF)
#define GET_REG(REG_NAME)    ( (REG_NAME) )



#define SET_BIT(VAR,BIT_NUM) (VAR) |=  (1 << (BIT_NUM))
#define CLR_BIT(VAR,BIT_NUM) (VAR) &= ~(1 << (BIT_NUM))
#define TOG_BIT(VAR,BIT_NUM) (VAR) ^=  (1 << (BIT_NUM))
#define GET_BIT(VAR,BIT_NUM) (((VAR) >> (BIT_NUM)) & 0x01)
#define BIT_IS_CLEAR(VAR, BIT_NUM) (!(VAR & (1<<BIT_NUM)))


#endif
