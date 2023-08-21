# Estação Meteorológica IoT com ESP32 e Display OLED.

## Descrição

Este projeto consiste em uma estação meteorológica IoT que utiliza um ESP32 para coletar e exibir dados de temperatura e umidade em um display OLED. Além disso, ele sincroniza automaticamente o horário através do protocolo NTP via conexão WiFi.

## Funcionalidades

- Coleta de dados de temperatura e umidade.
- Exibição das informações em um display OLED.
- Sincronização de horário via NTP.
- Interface simples e intuitiva.

## Vídeo

Confira o projeto em ação: [Assista ao Vídeo](https://youtube.com/shorts/4TjvYzfjJCk?feature=share)

## Como Usar

1. Conecte o ESP32: Certifique-se de que o ESP32 esteja conectado ao computador via cabo USB.

2. Abra a Arduino IDE: Abra a Arduino IDE no seu computador.

3. Abra o Projeto: Abra o projeto da estação meteorológica que você deseja carregar no ESP32.

4. Selecione a Porta: Na Arduino IDE, vá para a opção "Ferramentas" e selecione a porta serial adequada sob a opção "Porta". Isso deve ser a porta à qual o ESP32 está conectado.

5.Compile o Código: Clique no ícone de "checkmark" (verificação) na parte superior da janela da Arduino IDE para compilar o código.

6. Pressione o Botão de Reset: Agora, enquanto o código está sendo compilado e pouco antes de começar o processo de upload, pressione o botão de reset no ESP32.

7. Upload: Imediatamente após pressionar o botão de reset, clique no ícone de "seta" (upload) na parte superior da janela da Arduino IDE para enviar o código para o ESP32.

8. Aguarde: Aguarde enquanto o código é carregado no ESP32.

Se tudo correr bem, o código deve ser carregado com sucesso no ESP32. Se ocorrer algum erro durante o processo de upload, tente novamente seguindo os passos acima, incluindo o pressionamento do botão de reset.

Lembre-se de que as configurações específicas do seu ambiente podem afetar o processo de upload, então fique à vontade para ajustar o momento do pressionamento do botão de reset conforme necessário.
O display OLED exibirá as informações de temperatura, umidade e horário.
As informações de clima são obtidas via API da WeatherAPI.
O horário é sincronizado automaticamente através do protocolo NTP.

## Autor

Nome: Elismar Silva
GitHub: [ElismarSilva](https://github.com/ElismarSilva)

## Nota sobre Licença

Este projeto não possui uma licença definida. A ausência de uma licença implica que os direitos autorais estão reservados. Entre em contato com o autor se você estiver interessado em usar ou contribuir para este projeto.
