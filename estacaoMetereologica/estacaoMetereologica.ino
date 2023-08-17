
/*
Pinos do ESP32:

  D21 para SDA (dados)
  D22 para SCL (clock)
  GND ----->> GND
  VCC ----->> 3.3V

Projeto: Estação Meteorológica IoT com Visualização OLED e ESP32
Este projeto utiliza um ESP32 para criar uma estação meteorológica IoT, exibindo informações sobre temperatura e umidade em um display OLED. Além disso, o projeto obtém a hora atual através de sincronização NTP via WiFi.

Autor: Elismar Silva
Data: 17/08/2023
GitHub: https://github.com/ElismarSilva

Descrição:
A Estação Meteorológica IoT com Visualização OLED é um projeto que utiliza um microcontrolador ESP32 para coletar e exibir dados de temperatura e umidade. O display OLED mostra as informações de maneira clara e fácil de entender. Além disso, o projeto inclui sincronização de horário utilizando o protocolo NTP (Network Time Protocol) via conexão WiFi, garantindo que a hora exibida seja precisa.

Recursos do Projeto:
- Coleta de dados de temperatura e umidade.
- Exibição das informações em um display OLED.
- Sincronização de horário via NTP para garantir a precisão da hora.
- Utilização de conexão WiFi para obter dados de clima e sincronizar horário.
- Interface simples e intuitiva para visualização das informações.

Instruções de Uso:
1. Conecte os pinos do ESP32 conforme indicado acima.
2. Carregue o código fonte no ESP32 utilizando a Arduino IDE.
3. O display OLED exibirá as informações de temperatura, umidade e horário.
4. As informações de clima são obtidas através de uma API de previsão do tempo.
5. O horário é sincronizado automaticamente através do protocolo NTP.

Este projeto é uma demonstração do potencial do ESP32 como uma plataforma IoT para monitorar e exibir informações meteorológicas em tempo real. A visualização no display OLED torna as informações acessíveis e fáceis de ler.
*/



#include <SPI.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"
#include "sntp.h"

// Constantes para o display OLED
#define LARGURA_TELA 128
#define ALTURA_TELA 64
#define RESET_OLED     -1
#define ENDERECO_TELA 0x3C
#define LARGURA_IMAGEM 30
#define ALTURA_IMAGEM 30

// Objeto para o display OLED
Adafruit_SSD1306 tela(LARGURA_TELA, ALTURA_TELA, &Wire, RESET_OLED);

// Configuração da rede
const char nomeRede[] = {"**********"}; //Login de sua rede
const char senhaRede[] = {"********"}; //Senha de sua rede

const char* chaveAPI = "************************"; // Você pode acessar a WeatherAPI através do seguinte link: WeatherAPI. 
// Faça login e terar sua chave.

// Nomes das cidades para os dados do clima
const char* cidade_DF = "Brasilia";
const char cidades[15][20] = {
  "Brasilia", "Sao Paulo", "Rio de Janeiro", "Belo Horizonte",
  "Salvador", "Recife", "Fortaleza", "Manaus", "Natal",
  "Porto Alegre", "Curitiba", "Palmas", "Goiania", "Cuiaba", "Recife"
}; // Voce podera substituir por outras cidades

// Constantes relacionadas ao horário
const char* ntpServidor1 = "pool.ntp.org";
const char* ntpServidor2 = "time.nist.gov";
const long gmtOffset_seg = 3600;
const int ajusteHorarioVerao_seg = 3600;
const char* fusoHorario = "BRST+3BRDT+2,M10.3.0,M2.3.0";

// Abreviações para os dias da semana e meses
const char diasDaSemanaAbrev[7][4] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
const char mesesAbrev[12][4] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};

// Variáveis globais para o horário
int horas, minutos, segundos;

//Variaveis para mostrar temperatura e umidade local
int temperatura_DF, umidade_DF;
void setup() {
  Serial.begin(115200);
  tela.begin(SSD1306_SWITCHCAPVCC, ENDERECO_TELA);
  WiFi.begin(nomeRede, senhaRede);

  // Aguardar a conexão com a rede WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");

  // Configurar o SNTP (Protocolo de Hora na Rede Simples)
  sntp_set_time_sync_notification_cb(horarioDisponivel);
  sntp_servermode_dhcp(1);
  configTime(gmtOffset_seg, ajusteHorarioVerao_seg, ntpServidor1, ntpServidor2);
  configTzTime(fusoHorario, ntpServidor1, ntpServidor2);

  Serial.printf("Conectando a %s ", nomeRede);

  // Aguardar a conexão com a rede WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONECTADO");
}

void loop() {
  // Verificar a conexão com a rede WiFi
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Obter dados do clima para Brasília
    fetchWeatherData(http, cidade_DF, temperatura_DF, umidade_DF);

    // Obter dados do clima para outras cidades
    for (int i = 0; i < 12; i++) {
      int temperatura, umidade;
      fetchWeatherData(http, cidades[i], temperatura, umidade);

      // Armazenar dados para Brasília na primeira iteração
      if (i == 0) {
        temperatura_DF = temperatura;
        umidade_DF = umidade;
      }

      // Exibir dados do clima para todas as cidades (exceto Brasília)
      if (i != 0) {
        imprimirData(cidades[i], temperatura, umidade);
      }
    }

    // Exibir dados do clima para Brasília
    imprimirData(cidade_DF, temperatura_DF, umidade_DF);
    delay(5000);
  }
}

// Função para obter dados do clima usando a API
void fetchWeatherData(HTTPClient& http, const char* cidade, int& temperatura, int& umidade) {
  String url = "http://api.weatherapi.com/v1/current.json?key=" + String(chaveAPI) + "&q=" + String(cidade);
  http.begin(url);

  int codRespostaHTTP = http.GET();
  if (codRespostaHTTP == 200) {
    String dados = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, dados);

    temperatura = doc["current"]["temp_c"];
    umidade = doc["current"]["humidity"];

    http.end();
  }
}

// Função para exibir dados do clima no display OLED
void imprimirData(const char* cidade, int temperatura, int umidade) {
  // Exibir dados no monitor serial
  Serial.print(cidade);
  Serial.print("  ");
  Serial.print(temperatura);
  Serial.println("ºC");
  Serial.print("   ");

  // Exibir dados no display OLED
  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);
  tela.setCursor(0, 0);
  tela.print(cidade);
  tela.print(F(" "));
  tela.print(temperatura);
  tela.println(F(" C"));

  // Exibir horário atual
  struct tm informacaoHorario;
  if (getLocalTime(&informacaoHorario)) {
    char timeStr[6]; // Buffer para o horário formatado (HH:MM)
    sprintf(timeStr, "%02d:%02d", informacaoHorario.tm_hour, informacaoHorario.tm_min);

    tela.setTextSize(2);
    tela.setTextColor(SSD1306_WHITE);
    tela.setCursor(0, 10);
    tela.print(F("   "));
    tela.print(timeStr);

    // Exibir data
    tela.setTextSize(1);
    tela.setTextColor(SSD1306_WHITE);
    tela.setCursor(0, 30);
    tela.print(F(" "));
    tela.print(diasDaSemanaAbrev[informacaoHorario.tm_wday]);
    tela.print(F("., "));
    tela.print(informacaoHorario.tm_mday);
    tela.print(F(" "));
    tela.print(mesesAbrev[informacaoHorario.tm_mon]);
    tela.print(F(". "));
    tela.print(informacaoHorario.tm_year + 1900);

    // Exibir temperatura e umidade
    tela.setTextSize(2);
    tela.setTextColor(SSD1306_WHITE);
    tela.setCursor(0, 45);
    tela.print(temperatura_DF);

    tela.setTextSize(1);
    tela.setTextColor(SSD1306_WHITE);
    tela.setCursor(25, 43);
    tela.print(F("o"));

    tela.setTextSize(2);
    tela.setTextColor(SSD1306_WHITE);
    tela.setCursor(30, 45);
    tela.print("C    ");
    tela.print(umidade_DF);
    tela.println("%");
    tela.display();
    delay(1000);

    Serial.print(" ");
    Serial.print(mesesAbrev[informacaoHorario.tm_mon]);
    Serial.print("   ");
    Serial.print(temperatura_DF);
    Serial.println("ºC ");
    Serial.print(informacaoHorario.tm_year + 1900);
    Serial.print("     ");
    Serial.print(umidade_DF);
    Serial.print("%");
    Serial.println("    ");
    Serial.println();
  } else {
    Serial.println("Ainda não há horário disponível");
  }
}

// Função de retorno para sincronização de horário
void horarioDisponivel(struct timeval * t) {
  Serial.println("Horário ajustado pelo NTP!");
  struct tm informacaoHorario;

  // Obter horário local
  if (getLocalTime(&informacaoHorario)) {
    horas = informacaoHorario.tm_hour;
    minutos = informacaoHorario.tm_min;
    segundos = informacaoHorario.tm_sec;

    imprimirData("Brasilia", 0, 0); // Substituir pelos valores reais
  }
}
