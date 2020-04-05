// **********************************************************************
// PUCRS/FACIN
// Programa de testes para manipulação de Imagens
//
// Marcio Sarroglia Pinho
//
// pinho@pucrs.br
// **********************************************************************
//
//  Para rodar no XCODE, veja o arquivo "InstrucoesXCODE.rtf"
//


#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "ImageClass.h"
ImageClass Image, NewImage;

#include "Temporizador.h"
Temporizador T;


const int LIMIAR = 100;
#define LARGURA_JAN 1000
#define ALTURA_JAN 500

typedef struct // Cria uma STRUCT para armazenar os dados do histograma
{
     int vetor[255]; //a propria posição do vetor é a intensidade, o vetor armazena a quantidade
} Histograma;

Histograma hist;
// **********************************************************************
//  Cria o Histograma dependendo da imagem
// **********************************************************************
void PegaIntensidade(){
    double intensidade;
    for(int x = 0; x <= Image.SizeX(); x++){
        for(int y = 0; y <= Image.SizeY(); y++){
           intensidade = Image.GetPointIntensity(x,y);
           if(intensidade >= 10){ //ignora os pertos de 0(preto)
              hist.vetor[(int)intensidade] = hist.vetor[(int)intensidade] + 1;
              //printf("%f --- %f   ", intensidade, (double)hist.vetor[(int)intensidade]);
           }
        }
    }
    //printf("Termino cria hitograma");
}

void PegaIntensidadedoRetangulo(int x1,int y1,int x2, int y2,int x3,int y3, int x4, int y4){ //recebe um retangulo como ponto  1   2
                                                                                                                    //         3   4
    double intensidade;
    for(int x = x1; x <= x2; x++){
        for(int y = y1; y <= y3; y++){
           intensidade = Image.GetPointIntensity(x,y);
           if(intensidade >= 10){ //ignora os pertos de 0(preto)
              hist.vetor[(int)intensidade] = hist.vetor[(int)intensidade] + 1;
              //printf("%f --- %f   ", intensidade, (double)hist.vetor[(int)intensidade]);
           }
        }
    }
    //printf("Termino cria hitograma");
}

void DesenhaHistograma(){
    NewImage.DrawLineH(15,15,NewImage.SizeX()-10,0,0,0); //eixo X
    NewImage.DrawLineV(15,15,NewImage.SizeY()-10,0,0,0); //eixo Y
    int intensidade;
    int xinicial = 15;

    printf("Tamanho: %d\n\n",NewImage.SizeX()-25);

    for(int i=0;i<255;i++){
        intensidade = hist.vetor[i];
        intensidade = intensidade/5;
        if(intensidade>15){
            NewImage.DrawLineV(xinicial,15,intensidade,0,0,0);
            xinicial = xinicial + 3;
        }
    }
    glutPostRedisplay();
}

void DesenhaHistogramaRetangulo(){
    NewImage.DrawLineH(15,15,NewImage.SizeX()-10,0,0,0); //eixo X
    NewImage.DrawLineV(15,15,NewImage.SizeY()-10,0,0,0); //eixo Y
    int intensidade;
    int xinicial = 15;

    printf("Tamanho: %d\n\n",NewImage.SizeX()-25);

    for(int i=0;i<255;i++){
        intensidade = hist.vetor[i];
        intensidade = intensidade;
        if(intensidade>15){
            NewImage.DrawLineV(xinicial,15,intensidade,0,0,0);
            xinicial = xinicial + 3;
        }
    }
    glutPostRedisplay();
}


void Mediana(int qantpixel){ //altera entre 3,7,9 pixels usados
    PegaIntensidade();
    int vetmed[qantpixel];
    int teste[] = {2,80,6,2,5};
   // printf("Vetor inicial");
     //for(int u = 0; u < 5;u++){
    //printf("%d,  ",teste[u]);
// }

    //usado para exluir os pixels da ponta de serem o foco do algoritmo
    int kk; //posição do pixel do meio
    if(qantpixel == 3) {kk = 1;}
    if(qantpixel == 7) {kk = 3;}
    if(qantpixel == 9) {kk=4;}
    int j = 0;
    int ii = 0;
    int mediana = 0;
    for(int i = 0; i < 255 - kk;i++){
        j = 0;
        ii = i;
        while(j<qantpixel){//povoa o sub vetor
            vetmed[j] = hist.vetor[ii];
            //vetmed[j] = teste[ii];
            j++;
            ii++;
        }
        //organiza o vetor
        int a,b;
        for(int p = 0; p < qantpixel;p++){
            for(int pp = 0; pp < qantpixel;pp++){
                if(vetmed[pp]>vetmed[p]){
                    //troca
                    a = vetmed[pp];
                    b = vetmed[p];
                    vetmed[pp] = b;
                    vetmed[p] = a;
                }
            }
        }
        /*printf("Vetor Ordenado:");
         for(int u = 0; u < 5;u++){
            printf(" %d,  ",vetmed[u]);
            }*/
        //depois do vetor organizado pega o valor do meio
        mediana = vetmed[kk+1];
        hist.vetor[i] = mediana;
        //teste[i] = mediana;
    }
    DesenhaHistograma();
}

void DesenhaRetangulo(int x1,int y1,int x2, int y2,int x3,int y3, int x4, int y4){
    Image.DrawLineH(y1,x1,x2,255,255,255); //eixo X
    Image.DrawLineH(y3,x3,x4,255,255,255); //eixo X
    Image.DrawLineV(x1,y1,y4,255,255,255); //eixo Y
    Image.DrawLineV(x4,y4,y2,255,255,255); //eixo Y
     glutPostRedisplay();
     PegaIntensidadedoRetangulo(x1,y1,x2,y2,x3,y3,x4,y4);
}

void CriaHistogramaRetangulo(){
    for(int i = 0; i <= 255;i++){ //zera as posições do histograma para nao pegar sujeira
        hist.vetor[i] = 0;
    }
    DesenhaRetangulo(150,150,230,150,150,200,230,200);
    DesenhaHistogramaRetangulo();
}

void CriaHistograma(){
    for(int i = 0; i <= 255;i++){ //zera as posições do histograma para nao pegar sujeira
        hist.vetor[i] = 0;
    }
    PegaIntensidade();
    //DesenhaRetangulo(150,150,230,150,150,200,230,200);
    DesenhaHistograma();
}


// **********************************************************************
//  void ConvertBlackAndWhite()
// **********************************************************************
void ConvertBlackAndWhite()
{
    unsigned char r,g,b;
    int x,y;
    int i;

    cout << "Iniciou Black & White....";

    for(x=0; x<Image.SizeX(); x++)
    {
        for(y=0; y<Image.SizeY(); y++)
        {
            i = Image.GetPointIntensity(x,y); // VERIFICA O TOM DE CINZA DA IMAGEM
           // Image.ReadPixel(x,y,r,g,b);

            if (i < LIMIAR)
            {
                NewImage.DrawPixel(x, y,0,0,0);  // exibe um ponto PRETO na imagem
            }
            else NewImage.DrawPixel(x, y, 255,255,255); // exibe um ponto VERMELHO na imagem
        }
    }
    cout << "Concluiu Black & White.\n";
}
// **********************************************************************
// void DetectImageBorders()
// **********************************************************************
void ConvertBlackAndWhite(int Imin, int Imax)
{
    unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou Black & White....";
    for(x=0; x<Image.SizeX(); x++)
    {
        for(y=0; y<Image.SizeY(); y++)
        {
            i = Image.GetPointIntensity(x,y); // VERIFICA O TOM DE CINZA DA IMAGEM
            Image.ReadPixel(x,y,r,g,b);

            if (i >= Imin && i<= Imax)
            {
                NewImage.DrawPixel(x, y,0,0,0);  // exibe um ponto PRETO na imagem
            }
            else NewImage.DrawPixel(x, y, 255,255,255); // exibe um ponto VERMELHO na imagem

        }
    }
    cout << "Concluiu Black & White." << endl;
}

// **********************************************************************
// void DetectImageBorders()
// **********************************************************************
void DetectImageBorders()
{
    cout << "Iniciou DetectImageBorders....";
#define SENSIBILIDADE 10
    // varredura vertical

    int x,y;
    int i1,i2;
/*
    for(x=0; x<Image.SizeX(); x++)
    {
        for(y=0; y<Image.SizeY()-1; y++)
        {
            i1 = Image.GetPointIntensity(x,y); // Le o TOM DE CINZA DA IMAGEM
            i2 = Image.GetPointIntensity(x,y+1); // Le o TOM DE CINZA DA IMAGEM
            if (fabs(i1-i2) > SENSIBILIDADE)
                NewImage.DrawPixel(x, y,255,255,255);  // exibe um ponto BRANCO na imagem da direita
            else NewImage.DrawPixel(x, y,0,0,0);  // exibe um ponto BRANCO na imagem da direita

        }
    }
*/
    // varredura horizontal
    for(y=0; y<Image.SizeY(); y++)
    {
        for(x=0; x<Image.SizeX()-1; x++)
        {
            i1 = Image.GetPointIntensity(x,y); // Le o TOM DE CINZA DA IMAGEM
            i2 = Image.GetPointIntensity(x+1,y); // Le o TOM DE CINZA DA IMAGEM
            if (fabs(i1-i2) > SENSIBILIDADE)
                NewImage.DrawPixel(x, y,255,255,255);  // exibe um ponto BRANCO na imagem da direita
            else NewImage.DrawPixel(x, y,0,0,0);  // exibe um ponto BRANCO na imagem da direita

        }
    }

    cout << "Iniciou DetectImageBorders...." << endl;
}

// **********************************************************************
// void ConvertToGrayscale()
// **********************************************************************
void ConvertToGrayscale()
{
    cout << "Iniciou ConvertToGrayscale..." << endl;
    int x,y;
    int i;
    for(x=0; x<Image.SizeX(); x++)
    {
        for(y=0; y<Image.SizeY(); y++)
        {
            i = Image.GetPointIntensity(x,y); // Le o TOM DE CINZA DA IMAGEM
            NewImage.DrawPixel(x, y,i,i,i);  // exibe um ponto CINZA na imagem da direita
        }
    }
    cout << "Concluiu ConvertToGrayscale." << endl;
}

// **********************************************************************
// void InvertImage()
// **********************************************************************
void InvertImage()
{
    cout << "Iniciou InvertImage..." << endl;
    unsigned char r,g,b;
    int x,y;
    for(x=0; x<Image.SizeX(); x++)
    {
        for(y=0; y<Image.SizeY(); y++)
        {
            Image.ReadPixel(x,Image.SizeY()-y,r,g,b);
            NewImage.DrawPixel(x, y,r,g,b);  // exibe um ponto CINZA na imagem da direita
        }
    }

    cout << "Concluiu InvertImage." << endl;
}

// **********************************************************************
// void OrdenaVetor(int window[])
// **********************************************************************
void OrdenaVetor(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++)
    {
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--)
        {
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}
// **********************************************************************
// void MontaVetor(int Px, int Py, int Vetor[9])
// **********************************************************************
void MontaVetor(int Px, int Py, int Vetor[9])
{

}

// **********************************************************************
//  void init(void)
// **********************************************************************
void init()
{
    int r;
    // Carrega a uma imagem
    r = Image.Load("fatia01.bmp"); // Carrega uma imagem

    if (!r) exit(1); // Erro na carga da imagem
    else cout << ("Imagem carregada!\n");

    // Ajusta o tamanho da imagem da direita, para que ela
    // passe a ter o mesmo tamnho da imagem recem carregada
    //
    //Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da na chamada abaixo
    NewImage.SetSize(Image.SizeX(), Image.SizeY(), Image.Channels());
    cout << "Nova Imagem Criada" << endl;

}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
double AccumDeltaT=0;
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{
    double dt = T.getDeltaT();
    AccumDeltaT += dt;
    if (AccumDeltaT > 3) // imprime o frame rate a cada 3 segundos
    {
        AccumDeltaT =0;
        cout << "FPS: " << 1.0/dt << endl;
    }

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH)/2.0)/(double)Image.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
    Image.SetZoomH(zoomH);
    Image.SetZoomV(zoomV);

// posiciona a imagem no canto inferior esquerdo da janela
    Image.SetPos(0, 0);

// posiciona a imagem nova na metada da direita da janela
    NewImage.SetPos(glutGet(GLUT_WINDOW_WIDTH)/2, 0);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
    NewImage.SetZoomH(zoomH);
    NewImage.SetZoomV(zoomV);

// Coloca as imagens na tela
    Image.Display();
    NewImage.Display();

// Mostra a tela OpenGL
    glutSwapBuffers();

}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    cout << "Passou pela Keyboard."<< endl;
    switch ( key )
    {
    case 27: // Termina o programa qdo
        NewImage.Delete();
        Image.Delete();
        exit ( 0 );   // a tecla ESC for pressionada
        break;
    case '2':
//        ConvertBlackAndWhite(220, 255);
        ConvertBlackAndWhite();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    case 'g':
        ConvertToGrayscale();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;

    case 'b':
        DetectImageBorders();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    case 'i':
        InvertImage();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    case 'm':
        Mediana(9);
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    case 'c':
        NewImage.CopyTo(&Image);
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    case 'h':
        CriaHistograma();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    case 'r':
        CriaHistogramaRetangulo();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
    default:
        break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        break;
    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...

        break;
    default:
        break;
    }
}

// **********************************************************************
// **********************************************************************
void Mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        cout << "(" << x << "," << y << "): " ;
        cout << Image.GetPointIntensity(x,y) << endl;
        Image.DrawPixel(x,y,255,0,0);
        glutPostRedisplay();
    }


}

// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************

int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (10,10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow    ( "Image Loader" );

    init ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    glutMouseFunc(Mouse);

    glutIdleFunc ( display );

    glutMainLoop ( );
    return 0;
}


