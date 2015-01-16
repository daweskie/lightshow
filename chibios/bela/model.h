#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

/**

*/
void initModel(void);

/**

*/
void setAdValue(uint8_t channel, uint16_t value);

/**

*/
float getChannelMeasuredValue(uint8_t channel);

/**

*/
float getChannelNeedTemperature(uint8_t channel);

/**

*/
void setChannelNeedTemperature(uint8_t channel, float value);


#endif // MODEL_H_INCLUDED
