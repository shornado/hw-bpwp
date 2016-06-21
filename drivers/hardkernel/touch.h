//[*]--------------------------------------------------------------------------------------------------[*]
//
//
// 
//  I2C Touchscreen driver
//  2012.01.17
// 
//
//[*]--------------------------------------------------------------------------------------------------[*]
#ifndef _TOUCH_H_
#define _TOUCH_H_

//[*]--------------------------------------------------------------------------------------------------[*]
// extern function define
//[*]--------------------------------------------------------------------------------------------------[*]
extern	void 	touch_hw_reset		(struct touch *ts);
extern	int		touch_info_display	(struct touch *ts);
extern	int		touch_probe			(struct i2c_client *client);
extern	int 	touch_remove		(struct device *dev);

//[*]--------------------------------------------------------------------------------------------------[*]
#endif /* _TOUCH_H_ */

//[*]--------------------------------------------------------------------------------------------------[*]
//[*]--------------------------------------------------------------------------------------------------[*]
