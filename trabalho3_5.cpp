#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

int dim_x  = 36;
int dim_y  = 36;
int x_inicial = 166;
int y_inicial = 38;
int velocidades[4] = {5,5,5,5};
int VelocidadeMinima = 1000000;
int N1 = (VelocidadeMinima*((100.0 - ((velocidades[0]-1)*10.0))/100.0));
int N2 = (VelocidadeMinima*((100.0 - ((velocidades[1]-1)*10.0))/100.0));
int N3 = (VelocidadeMinima*((100.0 - ((velocidades[2]-1)*10.0))/100.0));
int N4 = (VelocidadeMinima*((100.0 - ((velocidades[3]-1)*10.0))/100.0));

float calcular_celocidade(int n){
  return (VelocidadeMinima*((100.0 - ((velocidades[0]-1)*10.0))/100.0));
}


//janela
sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

//fonte
sf::Font font;



//Variaveis do painel
sf::Texture texture1;
sf::Sprite sprite1;
sf::Texture texture2;
sf::Sprite sprite2;
sf::RectangleShape rectangle1;
sf::Text textV1(to_string(velocidades[0]), font);
sf::Text text1("TREM 1", font);
sf::Texture texture3;
sf::Sprite sprite3;
sf::Texture texture4;
sf::Sprite sprite4;
sf::RectangleShape rectangle2;
sf::Text textV2(to_string(velocidades[1]), font);
sf::Text text2("TREM 2", font);
sf::Texture texture5;
sf::Sprite sprite5;
sf::Texture texture6;
sf::Sprite sprite6;
sf::RectangleShape rectangle3;
sf::Text textV3(to_string(velocidades[2]), font);
sf::Text text3("TREM 3", font);
sf::Texture texture7;
sf::Sprite sprite7;
sf::Texture texture8;
sf::Sprite sprite8;
sf::RectangleShape rectangle4;
sf::Text textV4(to_string(velocidades[3]), font);
sf::Text text4("TREM 4", font);
sf::RectangleShape rectangle;
sf::Text text("Painel de Controle de Velocidade", font);
//Fim das Variaveis do painel_velocidade

//Variaveis do painel_trens
std::vector<sf::Sprite> quadrados_linha1;
sf::Texture trilho_horizontal;
sf::Texture trilho_vertical;
sf::Texture trem_verde;
sf::Texture trem_vermelho;
sf::Texture trem_azul;
sf::Texture trem_rosa;
sf::Sprite trem_1;
sf::Sprite trem_2;
sf::Sprite trem_3;
sf::Sprite trem_4;


sem_t sem1;//semáforos L3           -> 1
sem_t sem2;//semáforos L5           -> 1
sem_t sem3;//semáforos L4           -> 1
sem_t sem4;//semáforos L6           -> 1
sem_t sem5;//semáforos L10          -> 1
sem_t sem6;//semáforos L3 L4 L6     -> 2
sem_t sem7;//semáforos L5 L6 L10    -> 2
sem_t sem8;//semáforos L3 L4 L5 L10 -> 3

void run_trem_1(sf::Sprite *trem);
void run_trem_2(sf::Sprite *trem);
void run_trem_3(sf::Sprite *trem);
void run_trem_4(sf::Sprite *trem);

sf::Thread thread(&run_trem_1, &trem_1);	// create a thread
sf::Thread thread2(&run_trem_2, &trem_2);	// create a thread
sf::Thread thread3(&run_trem_3, &trem_3);	// create a thread
sf::Thread thread4(&run_trem_4, &trem_4);	// create a thread

void rotacao(sf::Sprite *trem,int x){
  switch (x) {
    case 1:
    trem->rotate(90);
    trem->move(0,-dim_y);
    break;
    case 2:
    trem->rotate(90);
    trem->move(dim_x,0);

    break;
    case 3:
    trem->rotate(90);
    trem->move(-dim_x,0);

    break;
    case 4:
    trem->rotate(90);
    trem->move(0,dim_y);
    break;
    case 5:
    trem->rotate(90);
    trem->move(dim_x,0);
    break;
    case 6:
    trem->rotate(90);
    trem->move(0,dim_y);

    break;
    case 7:
    trem->rotate(90);
    trem->move(-dim_x,0);

    break;
    case 8:
    trem->rotate(90);
    trem->move(0,-dim_y);
    break;



  }
}

void L1(sf::Sprite *trem, int *N){
  for(int i=0; i<3; i++)
  {
    trem->move(0,-dim_y);
    if(i==2)
    rotacao(trem,8);

    usleep(*N);
  }

}
void L2(sf::Sprite *trem, int *N){
  for(int i=0; i<3; i++)
  {
    trem->move(dim_x,0);
    usleep(*N);
  }
}
void L3(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==true){
    rotacao(trem,5);
    trem->move(dim_x,0);
    usleep(*N);
    for(int i=0; i<3; i++)
    {
      trem->move(0,dim_y);
      usleep(*N);
    }
  }
  else{
    trem->move(-dim_x,0);
    rotacao(trem,7);
    usleep(*N);
    sem_post(&sem6);
    sem_post(&sem4);
    sem_post(&sem7);
    for(int i=0; i<4; i++)
    {
      trem->move(0,-dim_y);
      if(i==3)
        rotacao(trem,8);
      usleep(*N);
    }
    sem_post(&sem1);
    sem_post(&sem8);
    trem->move(dim_x,0);
    usleep(*N);


  }
}
void L4(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==false)
  {
    trem->move(0,dim_y);
    rotacao(trem,6);
    usleep(*N);
    sem_post(&sem6);
    sem_post(&sem1);
    sem_post(&sem8);
    for(int i=0; i<4; i++)
    {
      if(i==0){
      }
      trem->move(-dim_x,0);
      if(i==3)
      rotacao(trem,7);
      usleep(*N);
    }
    sem_post(&sem3);

    trem->move(0,-dim_y);
    usleep(*N);

  }
  else{
    rotacao(trem,1);
    trem->move(0,-dim_y);
    usleep(*N);
    for(int i=0; i<3; i++)
    {
      trem->move(dim_x,0);
      usleep(*N);
    }
  }
}
void L5(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==true)  {
    rotacao(trem,5);
    trem->move(dim_x,0);
    usleep(*N);
    for(int i=0; i<3; i++)
    {
      trem->move(0,dim_y);
      usleep(*N);
    }
  }
  else
  {
    trem->move(-dim_x,0);
    rotacao(trem,7);
    usleep(*N);
    sem_post(&sem5);
    for(int i=0; i<4; i++)
    {
      trem->move(0,-dim_y);
      usleep(*N);
      if(i==3)
        rotacao(trem,8);
    }
    sem_post(&sem8);
    sem_post(&sem7);
    sem_post(&sem2);
    trem->move(dim_x,0);
    usleep(*N);
  }
}
void L6(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==false){
    trem->move(0,dim_y);
    rotacao(trem,6);
    usleep(*N);
    sem_post(&sem2);
    for(int i=0; i<3; i++)
    {
      trem->move(-dim_x,0);
      usleep(*N);
    }
  }
  else
  {
    for(int i=0; i<4; i++)
    {

      trem->move(dim_x,0);
      usleep(*N);
      if(i==0){
        sem_post(&sem6);
        sem_post(&sem3);
      }
    }
  }
}
void L7(sf::Sprite *trem, int *N,bool flag){
  for(int i=0; i<2; i++)
  {
    trem->move(dim_x,0);
    usleep(*N);
    if(i==0  && flag){
      // sem_post(&sem1);
      // sem_post(&sem8);
    }
  }
}
void L8(sf::Sprite *trem, int *N,bool flag){
  for(int i=0; i<2; i++)
  {
    trem->move(dim_x,0);
    usleep(*N);
    if(i==0 && flag){
      // sem_post(&sem8);
      // sem_post(&sem7);
      // sem_post(&sem2);
    }
  }
  rotacao(trem,5);
}
void L9(sf::Sprite *trem, int *N){
  trem->move(dim_x,0);
  usleep(*N);
  for(int i=0; i<3; i++)
  {
    trem->move(0,dim_y);
    usleep(*N);
  }
}
void L10(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==false){
    trem->move(0,dim_y);
    rotacao(trem,6);
    usleep(*N);
    for(int i=0; i<3; i++)
    {
      trem->move(-dim_x,0);
      usleep(*N);
    }
  }
  else
  {
    for(int i=0; i<5; i++)
    {
      trem->move(dim_x,0);
      usleep(*N);
      if(i==0){
        sem_post(&sem4);
        sem_post(&sem8);
      }
    }
    rotacao(trem,2);
    sem_post(&sem7);
    sem_post(&sem5);
    trem->move(0,dim_y);
    usleep(*N);

  }
}
void L11(sf::Sprite *trem, int *N){
  for(int i=0; i<3; i++)
  {
    trem->move(0,-dim_y);
    usleep(*N);
  }
}
void L12(sf::Sprite *trem, int *N){

  for(int i=0; i<3; i++)
  {
    trem->move(0,dim_y);
    usleep(*N);
  }
  rotacao(trem,4);
}
void L13(sf::Sprite *trem, int *N){
  for(int i=0; i<12; i++)
  {
    trem->move(-dim_x,0);
    usleep(*N);
  }
  rotacao(trem,3);
}

void variaveis_painel_velocidade(){

  texture1.loadFromFile("arrow-up.png");

  sprite1.setTexture(texture1);
  sprite1.setPosition(199, 520);


  texture2.loadFromFile("arrow-down.png");
  sprite2.setTexture(texture2);
  sprite2.setPosition(199, 450);


  rectangle1.setSize(sf::Vector2f(70, 28));
  rectangle1.setOutlineColor(sf::Color::Red);
  rectangle1.setOutlineThickness(1);
  rectangle1.setPosition(179, 487);


  textV1.setCharacterSize(25);
  textV1.setStyle(sf::Text::Bold);
  textV1.setFillColor(sf::Color::Black);
  textV1.setPosition(179+25,487);


  text1.setCharacterSize(15);
  text1.setStyle(sf::Text::Bold);
  text1.setFillColor(sf::Color::Green);
  text1.setPosition(185, 560);

  //TREM 2

  texture3.loadFromFile("arrow-up.png");

  sprite3.setTexture(texture3);
  sprite3.setPosition(331.67, 520);


  texture4.loadFromFile("arrow-down.png");

  sprite4.setTexture(texture4);
  sprite4.setPosition(331.67, 450);


  rectangle2.setSize(sf::Vector2f(70, 28));
  rectangle2.setOutlineColor(sf::Color::Red);
  rectangle2.setOutlineThickness(1);
  rectangle2.setPosition(312, 487);

  textV2.setCharacterSize(25);
  textV2.setStyle(sf::Text::Bold);
  textV2.setFillColor(sf::Color::Black);
  textV2.setPosition(312+25,487);

  text2.setCharacterSize(15);
  text2.setStyle(sf::Text::Bold);
  text2.setFillColor(sf::Color::Magenta);
  text2.setPosition(319, 560);

  //TREM 3
  texture5.loadFromFile("arrow-up.png");
  sprite5.setTexture(texture5);
  sprite5.setPosition(464.34, 520);


  texture6.loadFromFile("arrow-down.png");
  sprite6.setTexture(texture6);
  sprite6.setPosition(464.34, 450);

  rectangle3.setSize(sf::Vector2f(70, 28));
  rectangle3.setOutlineColor(sf::Color::Red);
  rectangle3.setOutlineThickness(1);
  rectangle3.setPosition(445, 487);


  textV3.setCharacterSize(25);
  textV3.setStyle(sf::Text::Bold);
  textV3.setFillColor(sf::Color::Black);
  textV3.setPosition(445+25,487);

  text3.setCharacterSize(15);
  text3.setStyle(sf::Text::Bold);
  text3.setFillColor(sf::Color::Red);
  text3.setPosition(452, 560);

  //TREM 4

  texture7.loadFromFile("arrow-up.png");
  sprite7.setTexture(texture7);
  sprite7.setPosition(597, 520);

  texture8.loadFromFile("arrow-down.png");
  sprite8.setTexture(texture8);
  sprite8.setPosition(597, 450);

  rectangle4.setSize(sf::Vector2f(70, 28));
  rectangle4.setOutlineColor(sf::Color::Red);
  rectangle4.setOutlineThickness(1);
  rectangle4.setPosition(578, 487 );


  textV4.setCharacterSize(25);
  textV4.setStyle(sf::Text::Bold);
  textV4.setFillColor(sf::Color::Black);
  textV4.setPosition(578+25,487);

  text4.setCharacterSize(15);
  text4.setStyle(sf::Text::Bold);
  text4.setFillColor(sf::Color::Blue);
  text4.setPosition(585, 560);

  //RETANGULO
  rectangle.setSize(sf::Vector2f(796, 198));
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(2, 400);

  //TEXTO
  text.setCharacterSize(25);
  text.setStyle(sf::Text::Bold);
  text.setFillColor(sf::Color::Black);
  text.setPosition(211, 400);
  //Fim do painel de Controle
}
void variaveis_painel_trens(){



  trilho_horizontal.loadFromFile("trilho_horizontal.png");
  trilho_vertical.loadFromFile("trilho_vertical.png");

  trem_verde.loadFromFile("trem_verde.png");
  trem_vermelho.loadFromFile("trem_vermelho.png");
  trem_azul.loadFromFile("trem_azul.png");
  trem_rosa.loadFromFile("trem_rosa.png");


  //Posicionando os trilhos
  for(int  j=0; j<3; j++){
    for (int i=0; i<13; i++)
    {
      sf::Sprite quadrado;
      quadrado.setTexture(trilho_horizontal);
      quadrado.setPosition(x_inicial+i*dim_x, y_inicial+j*dim_y*4);
      quadrados_linha1.push_back(quadrado);
    }
  }
  for(int  j=0; j<7; j++)  {
    if(j==3)
    {
      continue;
    }
    if(j<3)
    {
      for (int i=0; i<4; i++)
      {
        sf::Sprite quadrado;
        quadrado.setTexture(trilho_vertical);
        quadrado.setPosition(x_inicial+i*dim_x*4, y_inicial+dim_y+dim_y*j);
        quadrados_linha1.push_back(quadrado);
      }
    }
    else if(j>3)
    {
      for (int i=0; i<2; i++)
      {
        sf::Sprite quadrado;
        quadrado.setTexture(trilho_vertical);
        quadrado.setPosition(x_inicial+i*dim_x*4*3, y_inicial+dim_y+dim_y*j);
        quadrados_linha1.push_back(quadrado);
      }
    }
  }


  trem_1.setTexture(trem_verde);
  trem_1.setPosition(x_inicial, y_inicial);

  trem_2.setTexture(trem_rosa);
  trem_2.setPosition(x_inicial, y_inicial);
  trem_2.move(dim_x*5,0);

  trem_3.setTexture(trem_vermelho);
  trem_3.setPosition(x_inicial, y_inicial);
  trem_3.move(dim_x*9,0);

  trem_4.setTexture(trem_azul);
  trem_4.setPosition(x_inicial, y_inicial);
  trem_4.move(0,dim_y*8);
  trem_4.rotate(270);
  trem_4.move(0,36);

  //Ativiando as threads dos trens
}
void tela_trens(){
  for(int i=0; i<quadrados_linha1.size(); i++)
  {
    window.draw(quadrados_linha1[i]);
  }
  //Fim do desenho dos trilhos

  //Desenhando os trens
  window.draw(trem_1);
  window.draw(trem_2);
  window.draw(trem_3);
  window.draw(trem_4);
}
void tela_velocidades(){
  window.draw(rectangle);
  window.draw(text);
  window.draw(sprite1);
  window.draw(sprite2);
  window.draw(rectangle1);
  window.draw(textV1);
  window.draw(text1);
  window.draw(sprite3);
  window.draw(sprite4);
  window.draw(rectangle2);
  window.draw(text2);
  window.draw(sprite5);
  window.draw(sprite6);
  window.draw(rectangle3);
  window.draw(text3);
  window.draw(sprite7);
  window.draw(sprite8);
  window.draw(rectangle4);
  window.draw(text4);
  window.draw(textV2);
  window.draw(textV3);
  window.draw(textV4);
  //Fim da desenhando do painel de controle
}
void controle_velocidade(){
  sf::Vector2i position = sf::Mouse::getPosition(window);
  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
    //Velocidade do trem_1
    if(position.x>=179 && position.x<179+70){

      //Aumentar  nivel Velocidade do trem_1
      if(position.y>=487+30 && position.y<=487+30+30 ){

        if(velocidades[0]<10){
          velocidades[0]++;
          // N1 = (VelocidadeMinima*((100.0 - ((velocidades[0]-1)*10.0))/100.0));
          N1 = calcular_celocidade(velocidades[0]);
        }
        textV1.setString(to_string(velocidades[0]));
      }
      //Diminuir  nivel Velocidade do trem_1
      else if(position.y<=487 && position.y>=487-30 ){
        if(velocidades[0]>1){
          velocidades[0]--;
          // N1 = (VelocidadeMinima*((100.0 - ((velocidades[0]-1)*10.0))/100.0));
          N1 = calcular_celocidade(velocidades[0]);

        }
        textV1.setString(to_string(velocidades[0]));
      }

    }

    //Velocidade do trem_2
    else if(position.x>=312 && position.x<312+70){

      //Aumentar Velocidade
      if(position.y>=487+30 && position.y<=487+30+30 ){
        if(velocidades[1]<10){
          velocidades[1]++;
          N2 = (VelocidadeMinima*((100.0 - ((velocidades[1]-1)*10.0))/100.0));
        }

        textV2.setString(to_string(velocidades[1]));
      }

      //Diminuir Velocidade
      else if(position.y<=487 && position.y>=487-30 ){
        if(velocidades[1]>1){
          velocidades[1]--;
          N2 = (VelocidadeMinima*((100.0 - ((velocidades[1]-1)*10.0))/100.0));
        }
        textV2.setString(to_string(velocidades[1]));
      }
    }

    //Velocidade do trem_3
    else if(position.x>=445 && position.x<445+70){

      if(position.y>=487+30 && position.y<=487+30+30 ){
        if(velocidades[2]<10){
          velocidades[2]++;
          N3 = (VelocidadeMinima*((100.0 - ((velocidades[2]-1)*10.0))/100.0));
        }
        textV3.setString(to_string(velocidades[2]));
      }
      else if(position.y<=487 && position.y>=487-30 )
      {
        if(velocidades[2]>1)
        {
          velocidades[2]--;
          N3 = (VelocidadeMinima*((100.0 - ((velocidades[2]-1)*10.0))/100.0));
        }

        textV3.setString(to_string(velocidades[2]));
      }
    }

    //Velocidade do trem_4
    else if(position.x>=578 && position.x<578+70){
      if(position.y>=487+30 && position.y<=487+30+30 ){
        if(velocidades[3]<10){
          velocidades[3]++;
          N4 = (VelocidadeMinima*((100.0 - ((velocidades[3]-1)*10.0))/100.0));
        }
        textV4.setString(to_string(velocidades[3]));
      }
      else if(position.y<=487 && position.y>=487-30 ){
        if(velocidades[3]>1){
          velocidades[3]--;
          N4 = (VelocidadeMinima*((100.0 - ((velocidades[3]-1)*10.0))/100.0));
        }
        textV4.setString(to_string(velocidades[3]));
      }
    }
  }
}
void janela(){
  if (! font. loadFromFile ( "arial.ttf" ))
  {
      // erro ...
  }

  while (window.isOpen()){
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
        thread.terminate();
        thread2.terminate();
        thread3.terminate();
        thread4.terminate();
      }
      controle_velocidade();
    }
    window.clear();
    tela_velocidades();
    tela_trens();
    window.display();
  }

}

void inicializar_semaforos(){
  int res;
  res = sem_init(&sem1, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem2, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem3, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem4, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem5, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem6, 0, 2);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem7, 0, 2);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem8, 0, 3);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
}

int main(){

  inicializar_semaforos();
  variaveis_painel_velocidade();
  variaveis_painel_trens();
  thread.launch();	// run it
  thread2.launch();	// run it
  thread3.launch();	// run it
  thread4.launch();	// run it
  janela();



  return 0;
}

void run_trem_1(sf::Sprite *trem){
  while(1)
  {
    L2(trem, &N1);
    sem_wait(&sem8);
    sem_wait(&sem6);
    sem_wait(&sem1);

    L3(trem, &N1,true);
    sem_wait(&sem3);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem6);
    // sem_post(&sem8);
    // sem_post(&sem1);
    L4(trem, &N1,false);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem3);
    L1(trem, &N1);
  }
}
void run_trem_2(sf::Sprite *trem) {
  bool flag = false;
  while(1)
  {
    L7(trem, &N2,flag);
    sem_wait(&sem8);
    sem_wait(&sem7);
    sem_wait(&sem2);
    flag = true;

    L5(trem, &N2,true);
    sem_wait(&sem6);
    sem_wait(&sem4);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem2);

    L6(trem, &N2,false);
    sem_wait(&sem1);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem6);
    // sem_post(&sem4);
    // sem_post(&sem7);

    L3(trem, &N2,false);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem1);
    // sem_post(&sem8);
  }
}
void run_trem_3(sf::Sprite *trem){
  bool flag = false;
  while(1)
  {

    L8(trem, &N3 ,flag);
    flag = true;
    L9(trem, &N3);
    sem_wait(&sem8);
    sem_wait(&sem7);
    sem_wait(&sem5);


    L10(trem, &N3,false);
    sem_wait(&sem2);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem5);
    L5(trem, &N3,false);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem8);
    // sem_post(&sem7);
    // sem_post(&sem2);
  }
}
void run_trem_4(sf::Sprite *trem){
  while(1)
  {
    L11(trem, &N4);
    sem_wait(&sem8);
    sem_wait(&sem6);
    sem_wait(&sem3);

    L4(trem, &N4,true);
    sem_wait(&sem7);
    sem_wait(&sem4);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem6);
    // sem_post(&sem3);
    L6(trem, &N4,true);
    sem_wait(&sem5);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem4);
    // sem_post(&sem8);
    L10(trem, &N4,true);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem7);
    // sem_post(&sem5);
    L12(trem, &N4);

    L13(trem, &N4);
  }
}
