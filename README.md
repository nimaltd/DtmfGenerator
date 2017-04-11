# Dtmf Gerenator for Stm32 series Library
<br />
I hope use it and enjoy.
<br />
I use Stm32f103c8 and Keil Compiler and Stm32CubeMX wizard.
 <br />
Please Do This ...
<br />
<br />
1) Enable FreeRTOS or not.  
<br />
2) Config your timer and Select Pwm channel and enable Update interrupt on CubeMX 
<br />
3) Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.
<br />
4) Config your DtmfConfig.h file.
<br />
5) Add Dtmf_CallBack() on Timer interrupt routin.
<br />
6) call  Dtmf_Init() on your app.
<br />
7) use this function like Dtmf_Send() on your app. Example Dtmf_Send("1234aB",200,200);
