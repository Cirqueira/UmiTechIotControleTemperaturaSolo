# 🌾 UmiTechIot - Sistema Automatizado de Irrigação e Controle de Temperatura do Solo.

Sistema inteligente de monitoramento e automação de irrigação utilizando **ESP32**, protocolo **ESP-NOW** e arquitetura **MVC**.

## 🚀 Sobre o Projeto
O UmiTech foi desenvolvido para resolver o problema de comunicação em áreas onde o Wi-Fi convencional não alcança. Utilizando comunicação direta ponto-a-ponto, o sistema monitora a saúde do solo e do ar, decidindo o momento exato de acionar a irrigação.

### 🛠 Tecnologias e Protocolos
- **ESP-NOW:** Comunicação de baixa latência e longo alcance entre os módulos sem necessidade de roteador.
- **Padrão MVC:** Organização de software que separa a lógica de dados (Model), a interface web/física (View) e o processamento (Controller).
- **Web Server Assíncrono:** Interface de monitoramento em tempo real hospedada no próprio ESP32.

## 🏗 Arquitetura do Sistema

### 1. Nó Transmissor (Sensor)
- Leitura de sensor de umidade do solo (Higrômetro).
- Leitura de temperatura e umidade do ar (DHT22).
- Envio de pacotes de dados via ESP-NOW.

### 2. Nó Receptor (Mestre/Atuador)
- **Controller:** Processa os pacotes recebidos e gerencia o servidor web.
- **Model:** Mantém o estado atual e o histórico das últimas 10 leituras.
- **View:** 
    - **Digital:** Dashboard HTML/CSS moderno e responsivo.
    - **Física:** Acionamento de Relé para Bomba d'água.

## 🔌 Esquema de Ligação
> [Aqui você pode inserir o link para a imagem do seu circuito que está na pasta /docs]

## 💻 Instalação
1. Clone o repositório.
2. Instale as bibliotecas: `ESPAsyncWebServer`, `AsyncTCP`, `ArduinoJson`, `DHT sensor library`.
3. Descubra o MAC Address do seu Receptor e insira no código do Transmissor.
4. Faça o upload dos códigos respectivos para cada ESP32.

---
Desenvolvido por Leonard Cirqueira - 2026
