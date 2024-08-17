#include <Servo.h> // Inclui-se a Biblioteca para controle do servo motor

/////////////// Início - declaração das variáveis ///////////////

// Início - variávies físicas (portas digitais/analógicas)
int a = 4; // Atribui-se à variavél a porta lógica 4 do arduino. Refere-se ao segmento a do Visor de sete segmentos.
int b = 5; // Atribui-se à variavél b porta lógica 5 do arduino. Refere-se ao segmento b do Visor de sete segmentos.
int c = 6; // Atribui-se à variavél c porta lógica 6 do arduino. Refere-se ao segmento c do Visor de sete segmentos.
int d = 7; // Atribui-se à variavél d porta lógica 7 do arduino. Refere-se ao segmento d do Visor de sete segmentos.
int e = 8; // Atribui-se à variavél e porta lógica 8 do arduino. Refere-se ao segmento e do Visor de sete segmentos.
int f = 9; // Atribui-se à variavél f porta lógica 9 do arduino. Refere-se ao segmento f do Visor de sete segmentos.
int g = 10; // Atribui-se à variavél g porta lógica 10 do arduino. Refere-se ao segmento g do Visor de sete segmentos.
int buttonPin = 16; // Atribui-se à variavél buttonPin a porta analógica (utilizada como digital) 16 do arduino. Refere-se ao Botão.
int ledverde = 17; // Atribui-se à variavél ledverde a porta analógica (utilizada como digital) 17 do arduino. Refere-se ao LED verde.
int ledvermelho = 18; // Atribui-se à variavél ledvermelho a porta analógica (utilizada como digital) 18 do arduino. Refere-se ao LED vermelho.
int potenciomentro = A5; // Atribui-se à variavél potenciometro a porta analógica A5 do arduino. Refere-se ao potenciômetro.
int Buzzer = 15; // Atribui-se à variavél Buzzer a porta analógica (utilizada como digital) 15 do arduino. Refere-se ao potenciômetro.
Servo ServoFechadura; // Cria-se a variável ServoFechadura na biblioteca Servo.h. Refere-se ao microservo.
// Fim - variávies físicas (portas digitais/analógicas)

// Início - variávies lógicas
char senha[4] = {'2', '7', '3', '5'}; // Array (vetor) do tipo caractére, que contém a combinação correta.
char num[4]; // Array (vetor) do tipo caractére, que será construído com as inserções do usuário através do potenciômetro 
int leitura = 0; // Armazena
int ultleitura = 0; 
int contador = 0; // Contador
int Tone1 = 2132; // Frequência que compõe o som que o buzzer emitirá no caso da senha estar correta.
int Tone2 = 1989; // Frequência que compõe o som que o buzzer emitirá no caso da senha estar correta.
int Tone3 = 689; // Frequência que compõe o som que o buzzer emitirá no caso da senha estar incorreta.
int Tone4 = 679; // Frequência que compõe o som que o buzzer emitirá no caso da senha estar incorreta.
int k = 0; // Variável de controle do laço contado do som positivo do Buzzer
int w = 0; // Variável de controle do laço contado do som positivo do Buzzer
// Estados dos segmentos para os números de 0 a 9
byte numeros[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};
// Fim - variávies lógicas

/////////////// Fim - declaração das variáveis ///////////////


void setup() {
  pinMode(a, OUTPUT); // Inicializa a variável a como SAÍDA
  pinMode(b, OUTPUT); // Inicializa a variável b como SAÍDA
  pinMode(c, OUTPUT); // Inicializa a variável c como SAÍDA
  pinMode(d, OUTPUT); // Inicializa a variável d como SAÍDA
  pinMode(e, OUTPUT); // Inicializa a variável e como SAÍDA
  pinMode(f, OUTPUT); // Inicializa a variável f como SAÍDA
  pinMode(g, OUTPUT); // Inicializa a variável g como SAÍDA
  pinMode(ledverde, OUTPUT); // Inicializa a variável ledverde como SAÍDA
  pinMode(ledvermelho, OUTPUT); // Inicializa a variável ledevermelho como SAÍDA
  pinMode(Buzzer, OUTPUT); // Inicializa a variável Buzzer como SAÍDA
  pinMode(buttonPin, INPUT_PULLUP); // Configurando o botão com resistor de pull-up interno
  Serial.begin(9600); // Inicializa a comunicação serial

  ServoFechadura.attach(3); // Atribui o servomotor à porta 3 (PWM) do arduino  
  ServoFechadura.write(7); // Inicializa o microservo na posição 7º
  digitalWrite(ledverde, LOW);   // Inicializa o LED verde desligado
  digitalWrite(ledvermelho, LOW); // Inicializa o LED vermelhor desligado
}

void loop() {
  int displayNumber = map(analogRead(potenciomentro), 0, 1023, 0, 9); 
  /*A variável displayNumber recebe o resultado do remapeamento 
  do sinal analógico do potenciômetro para o sinal digital do visor de sete segmentos*/
  showNumber(displayNumber); // A função showNumber é chamada, sendo passado, como parâmetro a variável dispalyNumber

  leitura = digitalRead(buttonPin); // Realiza a leitura do sinal digital do botão e armazena o valor na variável leitura 
  Serial.println();
  delay(100);
  
  if (leitura == LOW && ultleitura == HIGH) { // Testa se o sinal da variável leitura é BAIXO e se o sinal da variável ultleitura é ALTO
    num[contador++] = displayNumber + '0'; // Constrói o array num com a variável contador sendo incrementada
    Serial.print("Número coletado: "); // Mostra a mensagem "número coletado: " no monitor serial
    Serial.println(displayNumber); // Mostra o número que foi coletado no monitor serial
    delay(1000); // Delay de 1 segundo

    if (contador == 4) {
      verificarSenha(); 
      contador = 0; // Reseta o contador após verificar a senha
    }
    delay(200); // Debounce para evitar múltiplas leituras do botão. Delay de 200 milissegundos
  }
  ultleitura = leitura; // A variável ultleitura recebe o valor da variável leitura
}

/////////////// Início - função para exibir os dígitos no dispaly /////////////// 
void showNumber(int num) {
  byte segmentos = numeros[num];

  digitalWrite(a, segmentos & 0b00000001);
  digitalWrite(b, segmentos & 0b00000010);
  digitalWrite(c, segmentos & 0b00000100);
  digitalWrite(d, segmentos & 0b00001000);
  digitalWrite(e, segmentos & 0b00010000);
  digitalWrite(f, segmentos & 0b00100000);
  digitalWrite(g, segmentos & 0b01000000);
}
/////////////// Fim - função para exibir os dígitos no dispaly /////////////// 


/////////////// Início - função para verificação da senha /////////////// 
void verificarSenha() {
  bool senhaCorreta = true; /* Atribui-se o valor lógico verdadeiro à variável senhaCorreta,
  então, assume-se que a senha está correta (é uma variável de comparação)*/ 
  for (int i = 0; i < 4; i++) { // Laço contado para verificação da combinação do cofre
    if (num[i] != senha[i]) { /* A partir de uma estrutura condicional, compara-se, elemento a elemento, o array (vetor) senha,
    que contém a combinação correta, com o array num, que é contruído de acordo com os
    dígitos que o usuário insere a partir do potenciômetro.*/
      senhaCorreta = false; /*Caso algum elemento do array
    num seja diferente do elemento de índice equivalente a ele no array senha, a variável
    de comparação receberá valor lógico falso. Do contrário, o valor lógico dela permanece inalterado*/
      break; // Para a execução do laço condicional
    }
  }

  if (senhaCorreta) { /* Estrutura condicional - verifica o valor da variável senhaCorreta (como ela foi declarada com valor lógico 
  verdadeiro, então, chama-se apenas ela no argumento, pois a negação dela tem valor lógico falso em relação ao inicial)*/
    Serial.println("Senha correta"); // Exibe a mensagem "Senha correta" no monitor serial
    digitalWrite(ledverde, HIGH); // LED verde acende
    for (k = 0; k < 3; k++){ // laço de repetição contada para controlar o toque positivo do Buzzer
        tone(Buzzer, Tone1); //Função tone com dois parâmetros: elemento e frequência.
        delay(130); // Delay de 130 milissegundos
        tone(Buzzer, Tone2); //Função tone com dois parâmetros: elemento e frequência.
        delay(120); // Delay de 120 milissegundos
    }
    tone(Buzzer, Tone2); //Função tone com dois parâmetros: elemento e frequência.
    delay(100); // Delay de 100 milissegundos
    noTone(Buzzer); // Para a função tone
    delay(1000); // Delay de 1 segundo
    ServoFechadura.write(90); // O servo motor é rotacionado em 90º
    delay(10000); // Mantém o microservo rotacionado e o LED verde aceso por 10 segundos
    ServoFechadura.write(0); // O servo motor volta para posição de 0º
    digitalWrite(ledverde, LOW); // LED verde é apagado

  } else {
    Serial.println("Senha incorreta"); // Exibe a mensagem "Senha incorreta" no monitor serial
    digitalWrite(ledvermelho, HIGH); // LED vermelho acende
      for (w = 0; w < 3; w++){ // Laço de repetição contada para controlar o toque negativo do Buzzer
          tone(Buzzer, Tone3); // Função tone com dois parâmetros: elemento e frequência.
          delay(130); // Delay de 120 milissegundos
          tone(Buzzer, Tone4); // Para a função tone
          delay(120); // // Delay de 120 milissegundos
      }
      tone(Buzzer, Tone3); // Função tone com dois parâmetros: elemento e frequência.
      delay(100); // Delay de 100 milissegundos
      noTone(Buzzer); // Para a função tone 
    delay(5000); // LED vermelho permanece aceso por 5 segundos
	digitalWrite(ledvermelho, LOW); // LED vermelho é apagado
  }
}
/////////////// Fim - função para verificação da senha /////////////// 
