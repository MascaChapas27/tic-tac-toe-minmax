#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

const int N = 3;

class Solucion {
public:
    int utilidad;
    int x, y;

    Solucion(int utilidad, int x, int y){
        this->utilidad = utilidad;
        this->x = x;
        this->y = y;
    }
};

class Estado {
public:
    char tablero[N][N];

    int max_valor(int a, int b){
        int terminal = test_terminal();
        if(terminal >= -1 && terminal <= 1) return terminal;

        int v = -999;

        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(this->tablero[i][j] == ' '){
                    Estado nuevoEstado(tablero);
                    nuevoEstado.setPieza(i,j,'o');
                    v = std::max(v,nuevoEstado.min_valor(a,b));

                    if(v >= b) return v;
                    a = std::max(a,v);

                }
            }
        }

        return v;
    }

    int min_valor(int a, int b){
        int terminal = test_terminal();
        if(terminal >= -1 && terminal <= 1) return terminal;

        int v = 999;

        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(this->tablero[i][j] == ' '){
                    Estado nuevoEstado(tablero);
                    nuevoEstado.setPieza(i,j,'x');
                    v = std::min(v,nuevoEstado.max_valor(a,b));

                    if(v <= a) return v;
                    b = std::min(b,v);

                }
            }
        }

        return v;
    }

    // 1 si ganas, -1 si pierdes, 0 si hay empate, 99 si nada
    int test_terminal(){

        // Guarda cuántas piezas hay puestas que no son espacios
        int total_piezas=0;

        // Horizontales
        for(int i=0;i<N;i++){
            int acum_x = 0;
            int acum_o = 0;
            for(int j=0;j<N;j++){
                if(tablero[i][j]=='x') acum_x++;
                if(tablero[i][j]=='o') acum_o++;
            }
            if(acum_x==N) return -1;
            if(acum_o==N) return 1;
            total_piezas = total_piezas + acum_x + acum_o;
        }

        // Verticales
        for(int i=0;i<N;i++){
            int acum_x = 0;
            int acum_o = 0;
            for(int j=0;j<N;j++){
                if(tablero[j][i]=='x') acum_x++;
                if(tablero[j][i]=='o') acum_o++;
            }
            if(acum_x==N) return -1;
            if(acum_o==N) return 1;
        }

        // Diagonal comenzando arriba/izquierda
        int acum_x = 0;
        int acum_o = 0;

        for(int i=0;i<N;i++){
            if(tablero[i][i]=='x') acum_x++;
            if(tablero[i][i]=='o') acum_o++;
        }

        if(acum_x==N) return -1;
        if(acum_o==N) return 1;

        // Diagonal comenzando arriba/derecha
        acum_x = 0;
        acum_o = 0;

        for(int i=0;i<N;i++){
            if(tablero[N-i-1][i]=='x') acum_x++;
            if(tablero[N-i-1][i]=='o') acum_o++;
        }

        if(acum_x==N) return -1;
        if(acum_o==N) return 1;

        if(total_piezas == N*N) return 0;

        return 99;
    }

    void setPieza(int x, int y, char pieza){
        this->tablero[x][y] = pieza;
    }

    Estado(char tablero[N][N]){

        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                this->tablero[i][j] = tablero[i][j];
            }
        }
    }

    void dibujaTablero(){

        std::cout << "  ";
        for(int i=0;i<N;i++){
            std::cout << " " << i << "  ";
        }

        std::cout << std::endl;

        std::cout << " +";
        for(int i=0;i<N;i++) std::cout << "---+";
        std::cout << std::endl;

        for(int i=0;i<N;i++){
            std::cout << i << "|";
            for(int j=0;j<N;j++){
                std::cout << " " << this->tablero[i][j] << " |";
            }
            std::cout << std::endl;
            std::cout << " +";
            for(int j=0;j<N;j++) std::cout << "---+";
            std::cout << std::endl;
        }
    }

    Solucion buscaSolucion(){

        int terminal = test_terminal();
        if(terminal >= -1 && terminal <= 1){
            return Solucion(terminal,-1,-1);
        }

        Solucion maximaSolucion(-999,-1,-1);

        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(this->tablero[i][j] == ' '){
                    Estado nuevoEstado(tablero);
                    nuevoEstado.setPieza(i,j,'o');
                    Solucion nuevaSolucion(0,i,j);
                    nuevaSolucion.utilidad = nuevoEstado.min_valor(-999,999);
                    if(nuevaSolucion.utilidad > maximaSolucion.utilidad)
                        maximaSolucion = nuevaSolucion;
                    if(nuevaSolucion.utilidad == maximaSolucion.utilidad && rand()%2)
                        maximaSolucion = nuevaSolucion;
                }
            }
        }

        if(maximaSolucion.x == -1)
            exit(EXIT_FAILURE);
        return maximaSolucion;
    }

    bool isDisponible(int x, int y){
        return tablero[x][y] == ' ';
    }
};

void clearScreen(){
    int status = 0;

    #ifdef _WIN32
        status = system("cls");
    #elif __linux__
        status = system("clear");
    #endif

    if(status!= 0) exit(EXIT_FAILURE);
}

int main(){

    srand(time(NULL));

    char tablero[N][N];

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            tablero[i][j] = ' ';
        }
    }

    Estado e(tablero);

    int terminal;

    while(true){
        int respuesta[2];
        do {
            clearScreen();

            e.dibujaTablero();

            std::cout << std::endl << "Introduce las coordenadas de tu jugada separadas por un espacio (x y): ";

            std::cin >> respuesta[0] >> respuesta[1];
        } while(respuesta[0] >= N || respuesta[1] >= N || respuesta[0] < 0 || respuesta[1] < 0 || !e.isDisponible(respuesta[1],respuesta[0]));

        e.setPieza(respuesta[1], respuesta[0], 'x');

        terminal = e.test_terminal();

        if(terminal >= -1 && terminal <= 1) break;

        Solucion s = e.buscaSolucion();

        e.setPieza(s.x,s.y,'o');

        terminal = e.test_terminal();

        if(terminal >= -1 && terminal <= 1) break;
    }

    clearScreen();

    e.dibujaTablero();

    std::cout << std::endl << std::endl;

    if(terminal == -1) std::cout << "Has ganado!";
    if(terminal == 0) std::cout << "Empate";
    if(terminal == 1) std::cout << "Has perdido...";
}
