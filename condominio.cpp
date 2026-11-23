#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
using namespace std;

const char* ARQ = "moradores.dat";
const char* TXT = "relatorio_moradores.txt";

struct Morador {
    int codigo;
    char nome[100], unidade[30], placa[15], situacao[10];
    bool ativo;
};

void cadastrar() {
    Morador m, temp;
    cout << "\nCodigo: ";
    cin >> m.codigo;
    cin.ignore();

    fstream arq(ARQ, ios::binary | ios::in | ios::out);
    bool achou = false;

    if (arq.is_open()) {
        while (arq.read((char*)&temp, sizeof(temp))) {
            if (temp.codigo == m.codigo) {
                if (temp.ativo) {
                    cout << "Codigo ja cadastrado!\n";
                    return;
                }
                achou = true;
                break;
            }
        }
        arq.close();
    }

    if (achou) {
        char resp;
        cout << "Codigo excluido. Deseja reutilizar? (S/N): ";
        cin >> resp;
        cin.ignore();
        if (resp != 'S' && resp != 's') return;
    }

    cout << "Nome: ";
    cin.getline(m.nome, 100);

    cout << "Unidade: ";
    cin.getline(m.unidade, 30);

    cout << "Placa: ";
    cin.getline(m.placa, 15);

    int op;
    cout << "Situacao (1-DENTRO / 2-FORA): ";
    cin >> op;
    cin.ignore();

    strcpy(m.situacao, op == 1 ? "DENTRO" : "FORA");
    m.ativo = true;

    if (achou) {
        fstream arquivo(ARQ, ios::binary | ios::in | ios::out);
        while (arquivo.read((char*)&temp, sizeof(temp))) {
            if (temp.codigo == m.codigo) {
                arquivo.seekp(-static_cast<streamoff>(sizeof(temp)), ios::cur);
                arquivo.write((char*)&m, sizeof(m));
                break;
            }
        }
        arquivo.close();
    } else {
        ofstream arquivo(ARQ, ios::binary | ios::app);
        arquivo.write((char*)&m, sizeof(m));
        arquivo.close();
    }

    cout << "Cadastro realizado!\n";
}

void consultar() {
    int codigo;
    cout << "Codigo: ";
    cin >> codigo;

    ifstream arq(ARQ, ios::binary);
    Morador m;
    bool achou = false;

    while (arq.read((char*)&m, sizeof(m))) {
        if (m.codigo == codigo && m.ativo) {
            cout << "\nNome: " << m.nome;
            cout << "\nUnidade: " << m.unidade;
            cout << "\nPlaca: " << m.placa;
            cout << "\nSituacao: " << m.situacao << "\n";
            achou = true;
            break;
        }
    }

    if (!achou) cout << "Morador nao encontrado!\n";
    arq.close();
}

void movimentar() {
    int codigo;
    cout << "Codigo: ";
    cin >> codigo;

    fstream arq(ARQ, ios::binary | ios::in | ios::out);
    Morador m;
    bool achou = false;

    while (arq.read((char*)&m, sizeof(m))) {
        if (m.codigo == codigo && m.ativo) {
            strcpy(m.situacao,
                strcmp(m.situacao, "DENTRO") == 0 ? "FORA" : "DENTRO");

            arq.seekp(-static_cast<streamoff>(sizeof(m)), ios::cur);
            arq.write((char*)&m, sizeof(m));

            cout << "Novo status: " << m.situacao << "\n";
            achou = true;
            break;
        }
    }

    if (!achou) cout << "Morador nao encontrado!\n";
    arq.close();
}

void excluir() {
    int codigo;
    cout << "Codigo: ";
    cin >> codigo;

    fstream arq(ARQ, ios::binary | ios::in | ios::out);
    Morador m;
    bool achou = false;

    while (arq.read((char*)&m, sizeof(m))) {
        if (m.codigo == codigo && m.ativo) {
            char resp;
            cout << "Excluir " << m.nome << "? (S/N): ";
            cin >> resp;

            if (resp == 'S' || resp == 's') {
                m.ativo = false;
                arq.seekp(-static_cast<streamoff>(sizeof(m)), ios::cur);
                arq.write((char*)&m, sizeof(m));
                cout << "Excluido!\n";
            }
            achou = true;
            break;
        }
    }

    if (!achou) cout << "Morador nao encontrado!\n";
    arq.close();
}

void exportar() {
    ifstream arq(ARQ, ios::binary);
    ofstream txt(TXT);
    Morador m;
    int total = 0;

    txt << "CODIGO | NOME | UNIDADE | PLACA | SITUACAO\n";

    while (arq.read((char*)&m, sizeof(m))) {
        if (m.ativo) {
            txt << m.codigo << " | " << m.nome << " | "
                << m.unidade << " | " << m.placa << " | "
                << m.situacao << "\n";
            total++;
        }
    }

    cout << total << " moradores exportados!\n";
    arq.close();
    txt.close();
}

void contabilizar() {
    ifstream arq(ARQ, ios::binary);
    Morador m;
    int ativos = 0, excluidos = 0, dentro = 0, fora = 0;

    while (arq.read((char*)&m, sizeof(m))) {
        if (m.ativo) {
            ativos++;
            if (strcmp(m.situacao, "DENTRO") == 0)
                dentro++;
            else
                fora++;
        } else {
            excluidos++;
        }
    }

    cout << "\nAtivos: " << ativos;
    cout << "\nDentro: " << dentro;
    cout << "\nFora: " << fora;
    cout << "\nExcluidos: " << excluidos << "\n";

    arq.close();
}

int main() {
    int op;

    do {
        cout << "\n===== CONTROLE DE MORADORES =====\n";
        cout << "1 - Cadastrar\n";
        cout << "2 - Consultar\n";
        cout << "3 - Exportar TXT\n";
        cout << "4 - Excluir\n";
        cout << "5 - Contabilizar\n";
        cout << "6 - Entrada/Saida\n";
        cout << "0 - Sair\n";
        cout << "Opcao: ";
        cin >> op;

        switch (op) {
            case 1: cadastrar(); break;
            case 2: consultar(); break;
            case 3: exportar(); break;
            case 4: excluir(); break;
            case 5: contabilizar(); break;
            case 6: movimentar(); break;
            case 0: cout << "Encerrando...\n"; break;
            default: cout << "Opcao invalida!\n";
        }

    } while (op != 0);

    return 0;
}
