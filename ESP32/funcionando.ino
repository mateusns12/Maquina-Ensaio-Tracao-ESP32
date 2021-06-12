#include "SPIFFS.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "driver/mcpwm.h"   
#include "driver/adc.h"
#include "HX711.h"
#include "esp_task_wdt.h"
#include "esp_task.h"
#include "rom/gpio.h"
#define GPIO_PWM0A_OUT 25   
#define GPIO_PWM0B_OUT 26
#define LED GPIO_NUM_2
#define ENABLE GPIO_NUM_27
#define interruptPin 18
#define DEBOUNCETIME 80
#define SSID "Tracao"
#define PASSWORD "12345678"
#define SERVER_PORT 80
#define ADC_TRESHOLD 800
#define DEBUG_SIGNAL 0
#define BIT_READ_TIMEOUT 100
#define PACKET_READ_TIMEOUT 250
#define PACKET_BITS 24
#define MIN_RANGE -(1<<20)
#define DRO_BUFFER_SIZE  0x1000

AsyncWebServer server(SERVER_PORT);

volatile int interruptCounter = 0;
volatile bool lastState; 
volatile uint32_t debounceTimeout = 0;
uint32_t saveDebounceTimeout;
bool saveLastState;
int save;

int dataPin = 36; // yellow
int clockPin = 39; // green

const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 17;

volatile float distance = 0;
volatile float load = 0;
volatile long int state = 0;

float tensao = 0;
float deformacao = 0;

static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne = 1;

float a_inicial = 0;
float c_inicial = 0;
volatile bool EN = 0;
bool flag = 0;
volatile bool pt = 0;
float dutycycle = 0;
char receivedCommand;
String menu = "\nw - web server\na - area inicial\nc - comprimento inicial\nd - dutycycle\nt - iniciar teste\ns - parar teste\nr - reverter motor\nu - finalizar reversao\nm - menu";

TaskHandle_t task_main;
TaskHandle_t task_teste;
TaskHandle_t task_parametros;
TaskHandle_t task_paq;
TaskHandle_t task_carga;
TaskHandle_t task_reverse;

HX711 scale;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

struct Reading
{  
    uint32_t timestamp;
    int32_t microns;
    Reading(): timestamp(millis()), microns(MIN_RANGE)  {}
    Reading& operator=(const Reading& obj) {
      timestamp = obj.timestamp;
      microns = obj.microns;
      return *this;
    }    
};

Reading dro_buffer[DRO_BUFFER_SIZE] = {};
size_t dro_index = 0;
size_t last_dro_index = 0;

void parametros (void *pvParameters);
void paquimetro (void *pvParameters);
void mainTask (void *pvParameters);
void reversao (void *pvParameters);
void loadCell (void *pvParameters);
void teste (void *pvParameters);

static void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
static void brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
static void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

static void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);  
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);     
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

static void brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);                    //Desliga o sinal do MCPWM no Operador A (Define o sinal em Baixo)
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);              //Configura a porcentagem do PWM no Operador B (Ciclo de trabalho)
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);  //define o nível do ciclo de trabalho (alto ou baixo)
}

static void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A); //Desliga o sinal do MCPWM no Operador A
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B); //Desliga o sinal do MCPWM no Operador B
}

void IRAM_ATTR handleButtonInterrupt() {
    portENTER_CRITICAL_ISR(&mux); 
      interruptCounter++;
      lastState = digitalRead(interruptPin);  
      debounceTimeout = xTaskGetTickCount();
    portEXIT_CRITICAL_ISR(&mux);
}

void initMCPWM(){
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);     
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT);
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 500;                          //frequência = 500Hz,
  pwm_config.cmpr_a = 0;                                //Ciclo de trabalho (duty cycle) do PWMxA = 0
  pwm_config.cmpr_b = 0;                                //Ciclo de trabalho (duty cycle) do PWMxb = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;           //Para MCPWM assimetrico
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}

void initWifi(){
  WiFi.softAP(SSID,PASSWORD);
  Serial.println(WiFi.softAPIP());  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });  
  server.on("/js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/js/highcharts.js", "text/javascript");
  });  
  server.on("/img", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/img/13301545.jpg", "image/jpeg");
  });
  server.on("/css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/style.css", "text/css");
  });
  server.on("/tracaoc",HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200,"text/plain",deformW().c_str());
  });
  server.on("/tracaod",HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200,"text/plain",cargaW().c_str());
  });
  server.begin();
}

String cargaW(){
  volatile double x = 0;
  portENTER_CRITICAL(&mux);
    x = load;
  portEXIT_CRITICAL(&mux);
  tensao = x/a_inicial;
  return String(tensao);
}

String deformW(){
  volatile double x = 0;
  portENTER_CRITICAL(&mux);
    x = distance;
  portEXIT_CRITICAL(&mux);
  deformacao = x/c_inicial;
  return String(deformacao);
}

void initPaquimetro(){
  Reading start_reading;
  dro_buffer[dro_index] = start_reading;
  analogReadResolution(11);  
  analogSetAttenuation(ADC_6db);
  adc1_config_width(ADC_WIDTH_BIT_10);
  pinMode(dataPin, INPUT);     //paquimetro
  pinMode(clockPin, INPUT);
}


void initLoadCell(){
  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println(scale.read());
  scale.set_scale(-22800.f);                      
  scale.tare();
}

void teste (void *pvParameters){
  Serial.println("Teste iniciado - Motor");
  esp_task_wdt_add(NULL);
  bool start = 0;
  int i=0;
  bool enable;
  while(true){
    portENTER_CRITICAL(&mux);      
      enable = EN;
    portEXIT_CRITICAL(&mux);
    if(enable == 1){
      if(start==0){
        for( i=0;i<=dutycycle;i++){
          brushed_motor_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, i);
          vTaskDelay(50/portTICK_PERIOD_MS);
          portENTER_CRITICAL(&mux);      
            enable = EN;
          portEXIT_CRITICAL(&mux);
          if(enable == 0){
            break;
          }
          if(i == dutycycle){
            start=1;
          }
        }
      }  
    }else{
      start = 0;
      brushed_motor_stop(MCPWM_UNIT_0,MCPWM_TIMER_0);
    }
    esp_task_wdt_reset();
  }
  esp_task_wdt_delete(NULL);
  vTaskDelete(NULL);  
}

void reversao (void *pvParameters){
  Serial.println("Reversao - Motor");
  esp_task_wdt_add(NULL);
  bool start = 0;
  int i=0;
  bool enable;
  while(true){
    portENTER_CRITICAL(&mux);      
      enable = EN;
    portEXIT_CRITICAL(&mux);
    if(enable == 1){
      if(start==0){
        for( i=0;i<=dutycycle;i++){
          brushed_motor_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, i);
          vTaskDelay(50/portTICK_PERIOD_MS);
          portENTER_CRITICAL(&mux);      
            enable = EN;
          portEXIT_CRITICAL(&mux);
          if(enable == 0){
            break;
          }
          if(i == dutycycle){
            start=1;
          }
        }
      }  
    }else{
      start = 0;
      brushed_motor_stop(MCPWM_UNIT_0,MCPWM_TIMER_0);
    }
    esp_task_wdt_reset();
  }
  esp_task_wdt_delete(NULL);
  vTaskDelete(NULL);  
}

void parametros (void *pvParameters){
  Serial.println("Task parametro");
  //esp_task_wdt_add(NULL);
  volatile float f = 0;
  volatile float d = 0;
  float carga;
  float deformP;
  bool enable = 0;
  while(true){
    portENTER_CRITICAL(&mux);      
      f = load;
      d = distance;
      enable = pt;
    portEXIT_CRITICAL(&mux);
    carga = f*9.81/(1000*a_inicial);
    deformP = d/c_inicial;
    if(enable==1){
      //Serial.println(String(f)+"\t"+String(d));
      Serial.println(String(carga)+"\t"+String(deformP));
      //Serial.println(String(f)+"\t"+String(d)+"\t"+String(carga)+"\t"+String(deformP));
    }    
    vTaskDelay(200/portTICK_PERIOD_MS);
    //esp_task_wdt_reset();
  }//esp_task_wdt_delete(NULL);
  vTaskDelete(NULL);
}

void loadCell (void *pvPArameters) {
  Serial.println("Task Celula de carga");
  scale.tare();
  Serial.println("Escala Zerada");
  esp_task_wdt_add(NULL);
  volatile double x;
  while(true){
    //Serial.print((scale.get_value(1)/-22800.f));
    x = scale.get_value(1)/-22800.f;
    portENTER_CRITICAL(&mux);      
      load = x;
    portEXIT_CRITICAL(&mux);    
    if(load >= 180){
      gpio_set_level(ENABLE,0);      
    }
    esp_task_wdt_reset();
  }esp_task_wdt_delete(NULL);
  vTaskDelete(NULL);
}

void mainTask(void *pvParameters){
  Serial.println("Task Listener");
  esp_task_wdt_add(NULL);
  while(true){
    portENTER_CRITICAL_ISR(&mux);
      save  = interruptCounter;
      saveDebounceTimeout = debounceTimeout;
      saveLastState  = lastState;
    portEXIT_CRITICAL_ISR(&mux);
    bool currentState = digitalRead(interruptPin);
    if(currentState != saveLastState){
      saveDebounceTimeout = millis();
    }  
    if((millis() - saveDebounceTimeout) > DEBOUNCETIME && (save != 0)){
    if(currentState){
      //Serial.println("Parado");
      portENTER_CRITICAL(&mux);
        EN = 0;
        pt = 0;
      portEXIT_CRITICAL(&mux);
      }else{
      //Serial.println("Acionado");
      portENTER_CRITICAL(&mux);
        EN = 1;
        pt = 1;
      portEXIT_CRITICAL(&mux);
      }
      portENTER_CRITICAL_ISR(&mux);  //início da seção crítica
        interruptCounter = 0; 
      portEXIT_CRITICAL_ISR(&mux);
    }esp_task_wdt_reset();
  } 
  esp_task_wdt_delete(NULL);
  vTaskDelete(NULL);
}

int getBit() {
    int data;        
    int readTimeout = millis() + BIT_READ_TIMEOUT;
    while (analogRead(clockPin) > ADC_TRESHOLD) {
      if (millis() > readTimeout)
        return -1;
    }    
    while (analogRead(clockPin) < ADC_TRESHOLD) {
      if (millis() > readTimeout)
        return -1;
    }    
    data = (analogRead(dataPin) > ADC_TRESHOLD)?1:0;
    return data;
}

long getPacket() 
{
    long packet = 0;
    int readTimeout = millis() + PACKET_READ_TIMEOUT;
    int bitIndex = 0;
    while (bitIndex < PACKET_BITS) {
      int bit = getBit();
      if (bit < 0 ) {
        // bit read timeout: reset packet or bail out
        if (millis() > readTimeout) {
          // packet timeout
          return -1;
        }        
        bitIndex = 0;
        packet = 0;
        continue;
      }
      packet |= (bit & 1)<<bitIndex;      
      bitIndex++;
      esp_task_wdt_reset();
    }    
    return packet;
}

long getMicrons(long packet)
{
  if (packet < 0)
    return MIN_RANGE;        
  long data = (packet & 0xFFFFF)*( (packet & 0x100000)?-1:1);
  if (packet & 0x800000) {
        // inch value (this comes sub-sampled) 
        data = data*254/200;
  }
  return data;
}

void paquimetro(void *pvParameters )
{
  Serial.println("Task Paquimetro");
    esp_task_wdt_add(NULL);
    volatile double y = 0;
    uint32_t lastReadTime = millis();
    while(true) { 
      long packet = getPacket() ;     
      if (packet < 0) {
        // read timeout, display?
        if (millis() > lastReadTime + PACKET_READ_TIMEOUT) {
          // advance last read to time-out
          lastReadTime = millis();
        }
      } else {
        size_t new_dro_index = (dro_index+1) % DRO_BUFFER_SIZE;
        dro_buffer[new_dro_index].timestamp = millis();
        dro_buffer[new_dro_index].microns = getMicrons(packet);
        dro_index = new_dro_index;
        y = dro_buffer[new_dro_index].microns;
        portENTER_CRITICAL(&mux);
          distance = y; 
        portEXIT_CRITICAL(&mux);           
      }esp_task_wdt_reset();
    }esp_task_wdt_delete(NULL);
    vTaskDelete( NULL ); 
}

void setup() {
 
  Serial.begin(115200);
  
  gpio_set_direction(LED,GPIO_MODE_OUTPUT);
  gpio_set_direction(ENABLE,GPIO_MODE_OUTPUT);
  gpio_set_level(LED,0);
  gpio_set_level(ENABLE,0);
        
  initMCPWM();
  initPaquimetro();
  initLoadCell();

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleButtonInterrupt, CHANGE);

  esp_task_wdt_init(20,true);
  disableCore0WDT();
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("Setup Finalizado");
  Serial.println("");
  Serial.println(menu);
}

void loop() {
  CheckSerialSwitchCase();
}

void CheckSerialSwitchCase() //Checking the serial port via Switch-Case 
{
  if (Serial.available()) { //If there are bytes/characters coming through the serial port
    char commandcharacter = Serial.read(); //read the character for the command    
    switch (commandcharacter) { //we read a character which can 
    case 'w':    
      initWifi();
      vTaskDelay(1000/portTICK_PERIOD_MS);
      Serial.println("Wifi habilitado");
      break;

    case 'm':    
      Serial.println(menu);
      break;

    case 'r':    
      if(flag==0){
        xTaskCreatePinnedToCore(mainTask,"mainTask",10000,NULL,3,&task_main,taskCoreZero);
        vTaskDelay(200/portTICK_PERIOD_MS);
        xTaskCreatePinnedToCore(reversao,"reversao",10000,NULL,3,&task_reverse,taskCoreZero);
        vTaskDelay(200/portTICK_PERIOD_MS);
        gpio_set_level(ENABLE,1);
      }else{
        Serial.println("Finalize o teste");
      }
      break;

    case 'u':      
      gpio_set_level(ENABLE,0);
      flag = 1;
      esp_task_wdt_delete(task_reverse);
      vTaskDelete(task_reverse);
      vTaskDelay(200/portTICK_PERIOD_MS);
      esp_task_wdt_delete(task_main);
      vTaskDelete(task_main);   
      vTaskDelay(200/portTICK_PERIOD_MS);      
      Serial.println("Reversao finalizada");   
      break;
      
    case 'a':    
    while (!Serial.available());      
      a_inicial = Serial.parseFloat();
      Serial.println("area inicial: "+String(a_inicial)+" mm2");
      break;

    case 'c':    
      while (!Serial.available());      
      c_inicial = Serial.parseFloat();
      Serial.println("comprimento inicial: "+String(c_inicial)+" mm");
      break;

    case 'd': //example: a 42 
      while (!Serial.available());      
      dutycycle = Serial.parseFloat();      
      Serial.println("dutyclycle: "+String(dutycycle)+"%");
      break;

    case 't': //inicia teste
      Serial.println("");
      xTaskCreatePinnedToCore(mainTask,"mainTask",10000,NULL,3,&task_main,taskCoreZero);
      vTaskDelay(200/portTICK_PERIOD_MS);
      xTaskCreatePinnedToCore(teste,"teste",10000,NULL,3,&task_teste,taskCoreZero);
      vTaskDelay(200/portTICK_PERIOD_MS);
      xTaskCreatePinnedToCore(paquimetro,"paquimetro",10000,NULL,3,&task_paq,taskCoreZero);
      vTaskDelay(200/portTICK_PERIOD_MS);
      xTaskCreatePinnedToCore(loadCell,"loadCell",10000,NULL,3,&task_carga,taskCoreZero);
      vTaskDelay(200/portTICK_PERIOD_MS);
      xTaskCreatePinnedToCore(parametros,"parametros",10000,NULL,4,&task_parametros,taskCoreOne);
      vTaskDelay(200/portTICK_PERIOD_MS);
      gpio_set_level(ENABLE,1);
      flag = 1;
      break;            

    case 's': //para teste      
      gpio_set_level(ENABLE,0);
      flag = 0;
      esp_task_wdt_delete(task_main);
      vTaskDelete(task_main);
      vTaskDelete(task_parametros);      
      esp_task_wdt_delete(task_teste);
      vTaskDelete(task_teste);
      esp_task_wdt_delete(task_paq);
      vTaskDelete(task_paq);
      esp_task_wdt_delete(task_carga);
      vTaskDelete(task_carga);
      Serial.println("teste finalizado");
      break;
    }
  } 
}
