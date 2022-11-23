/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//delay function
//delay function in microseconds
void delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim4))<time);
}


//Function to display temperature
void Display_Temp (float Temp)
{
	char str[20] = {0};
	lcd_put_cur(0,0);

	sprintf (str, "TEMP- %.2f", Temp);
	lcd_send_string(str);
	lcd_send_data('c');
}

//function to display RH
void Display_Rh (float Rh)
{
	char str[20] = {0};
	lcd_put_cur(1,0);

	sprintf (str, "HUMID:- %.2f", Rh);
	lcd_send_string(str);
	lcd_send_data('%');
}

//variables declarations
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;

float Temperature = 0;
float Humidity = 0;
uint8_t Presence = 0;

//function to set the pins as outputs
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

//function to set pin as input
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL; //can be changed to PULLUP if no data is received from the pin
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/***************************DHT11 FUNCTIONS BEGIN HERE**********************************************/
//Define the pin and the port for DHT11 Sensor
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_5

//Function to send the start signal from DHT11 data sheet
void DHT11_Start (void)
{
	Set_Pin_Output (DHT11_PORT, DHT11_PIN); //set the dht pin as output
	/***********************************************/
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 1); //initialize with data pin high
	HAL_Delay(1000); //wait for 1000 milliseconds
	/***********************************************/

	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0); //pull the pin low
	delay(18000); //wait 18 milliseconds
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1); //pull the pin high
	delay(20); //wait for 20 microseconds
	Set_Pin_Input(DHT11_PORT, DHT11_PIN); //set the pin as input
}

//dh11 function to check response
uint8_t DHT11_Check_Response (void)
{
	uint8_t Response = 0;
	delay(40); //first wait for 40 microseconds
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) //check for pin to be low
	{
		delay(80); //wait for 80 microseconds
		if((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1; //check if pin is high and return 1 to show sensor is present
		else Response = -1; //255
	}
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))); //wait for the pin to go low again

	return Response;
}

//function to read data from dht11 signal pin
uint8_t DHT11_Read (void)
{
	uint8_t i, j;
	for (j=0;j<8;j++)
	{
		while(!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))); //wait for the pin to change to high
		delay(40); //wait for 40 microseconds
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) //if the pin is low
		{
			i&= ~(1<<(7-j)); //write 0
		}
		else i|= (1<<(7-j)); //if the pin is high write 1
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))); //wait for the pin to go low
	}

	return i;
}
/* USER CODE END 0 */

/* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

		Display_Temp(Temperature);
		Display_Rh(Humidity);

		/*******DHT11*********/
		DHT11_Start();
		Presence = DHT11_Check_Response(); //record the response from the sensor

		//Five bytes of data
		Rh_byte1 = DHT11_Read ();
		Rh_byte2 = DHT11_Read ();
		Temp_byte1 = DHT11_Read ();
		Temp_byte2 = DHT11_Read ();
		SUM = DHT11_Read ();

		TEMP = Temp_byte1;
		RH = Rh_byte1;

		Temperature = (float) TEMP;
		Humidity = (float) RH;


		HAL_Delay(1200);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
