#ifndef MODEL_H
#define MODEL_H

struct DadosPlanta {
    float tempAr;
    int umidSolo;
    bool bombaAtiva;
};

DadosPlanta statusAtual = {0.0, 0, false};
DadosPlanta historico[10];
int indiceH = 0;

void adicionarAoHistorico(DadosPlanta d) {
    historico[indiceH] = d;
    indiceH = (indiceH + 1) % 10;
}

bool avaliarRega(int solo) {
    // Lógica baseada na sua escala: 801-1023 é seco
    return (solo > 800); 
}

#endif