/*-----------------------------------------

--------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h> //-- -lm
#include <stdbool.h> //bool

#include "/home/midas/ctest/kmods/soopwm/sooall_pwm.h"

#define PWM_DEV "/dev/sooall_pwm"

int main(int argc, char *argv)
{
	int ret = -1;
	int pwm_fd;
	int pwmno;
	struct pwm_cfg  cfg;
	int tmp;

	pwm_fd = open(PWM_DEV, O_RDWR);
	if (pwm_fd < 0) {
		printf("open pwm fd failed\n");
		return -1;
	}

	cfg.no        =   0;    /* pwm0 */
	cfg.clksrc    =   PWM_CLK_40MHZ; //40MHZ or 100KHZ;
	cfg.clkdiv    =   PWM_CLK_DIV32; //DIV2 40/2=20MHZ
	cfg.old_pwm_mode =true;    /* old mode  */
	cfg.idelval   =   0;
	cfg.guardval  =   0;
	cfg.guarddur  =   0;
	cfg.wavenum   =   0;  /* forever loop */
	cfg.datawidth =   5000;//781;//--limit 2^13-1=8191 
	cfg.threshold =   5000; //500;
	//---period=1000/100(KHZ)*(DIV(1-128))*datawidth   (us)
	//---period=1000/40(MHz)*(DIV)*datawidth       (ns)
        if(cfg.clksrc == PWM_CLK_100KHZ)
		{
			tmp=pow(2.0,(float)(cfg.clkdiv));
			printf("tmp=%d,set PWM period=%d us\n",tmp,(int)(1000.0/100.0*tmp*(int)(cfg.datawidth))); // div by integer is dangerous!!!
		}
        else if(cfg.clksrc == PWM_CLK_40MHZ)
		{
			tmp=pow(2.0,(float)(cfg.clkdiv));
			printf("tmp=%d,set PWM period=%d ns\n",tmp,(int)(1000.0/40.0*tmp*(int)(cfg.datawidth))); // div by integer is dangerous!!!
		}

	ioctl(pwm_fd, PWM_CONFIGURE, &cfg);
	ioctl(pwm_fd, PWM_ENABLE, &cfg);

        while(1){
		for(tmp=0;tmp<5000;tmp++){
			cfg.threshold=tmp;
			usleep(200);
			ioctl(pwm_fd,PWM_CONFIGURE,&cfg);}
		for(tmp=5000;tmp>5;tmp--){
			cfg.threshold=tmp;
			usleep(200);
			ioctl(pwm_fd,PWM_CONFIGURE,&cfg);}
		}
/*
	while (1) {
		static int cnt = 0;
		sleep(5);
		ioctl(pwm_fd, PWM_GETSNDNUM, &cfg);
		printf("send wave num = %d\n", cfg.wavenum);
		cnt++;
		if (cnt == 10) {
			ioctl(pwm_fd, PWM_DISABLE, &cfg);
			break;
		}
	}
*/
	return 0;
}