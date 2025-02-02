#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <locale>

class Pessoa {
private:
    int idade;
    std::string nome;

public:
    Pessoa(int idade, std::string nome) : idade(idade), nome(nome) {}

    int getIdade() const { return idade; }
    void setIdade(int idade) { this->idade = idade; }

    std::string getNome() const { return nome; }
    void setNome(std::string nome) { this->nome = nome; }
};

class Aluno : public Pessoa {
private:
    std::string matricula;
    std::string turma;

    bool validarMatricula(const std::string& matricula) const {
        return matricula.length() == 10 && std::all_of(matricula.begin(), matricula.end(), ::isdigit);
    }

    bool validarIdade(int idade) const {
        return idade >= 16 && idade <= 60;
    }

    bool validarTurma(const std::string& turma) const {
        return turma.find("Engenharia") == 0;
    }

public:
    Aluno(int idade, std::string nome, std::string matricula, std::string turma)
        : Pessoa(idade, nome), matricula(matricula), turma(turma) {
        if (!validarIdade(idade)) throw std::invalid_argument("Idade inválida! Deve estar entre 16 e 60 anos.");
        if (!validarMatricula(matricula)) throw std::invalid_argument("Matrícula inválida! Deve ter 10 dígitos.");
        if (!validarTurma(turma)) throw std::invalid_argument("Turma inválida! Deve começar com 'Engenharia'.");
    }

    std::string getMatricula() const { return matricula; }
    void setMatricula(const std::string& matricula) {
        if (!validarMatricula(matricula)) throw std::invalid_argument("Matrícula inválida!");
        this->matricula = matricula;
    }

    void setIdade(int idade) {
        if (!validarIdade(idade)) throw std::invalid_argument("Idade inválida!");
        Pessoa::setIdade(idade);
    }

    std::string getTurma() const { return turma; }
    void setTurma(const std::string& turma) {
        if (!validarTurma(turma)) throw std::invalid_argument("Turma inválida!");
        this->turma = turma;
    }
};

class RankingTurmas {
private:
    std::map<std::string, int> pontuacaoGeral;

public:
    void adicionarPontuacao(const std::string& turma, int pontos) {
        pontuacaoGeral[turma] += pontos;
    }

    std::string getTurmaVencedora() const {
        auto it = std::max_element(pontuacaoGeral.begin(), pontuacaoGeral.end(),
                                   [](const auto& a, const auto& b) { return a.second < b.second; });
        return (it != pontuacaoGeral.end()) ? it->first : "Nenhuma";
    }

    void exibirRanking() const {
        std::vector<std::pair<std::string, int>> ranking(pontuacaoGeral.begin(), pontuacaoGeral.end());

        std::sort(ranking.begin(), ranking.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        std::cout << "Ranking das Turmas:\n";
        for (const auto& par : ranking) {
            std::cout << "Turma " << par.first << ": " << par.second << " pontos\n";
        }
    }
};

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    try {
        Aluno aluno1(17, "Carlos Silva", "1234567890", "Engenharia Civil");
        std::cout << "Aluno criado com sucesso: " << aluno1.getNome() << ", Turma: " << aluno1.getTurma() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Erro ao criar aluno1: " << e.what() << '\n';
    }

    try {
        Aluno aluno2(15, "Maria Oliveira", "1234567890", "Engenharia Elétrica");  // Deve falhar
    } catch (const std::exception& e) {
        std::cerr << "Erro ao criar aluno2: " << e.what() << '\n';
    }

    RankingTurmas ranking;
    ranking.adicionarPontuacao("Engenharia Civil", 100);
    ranking.adicionarPontuacao("Engenharia Mecânica", 80);
    ranking.exibirRanking();
    
    std::cout << "Turma vencedora: " << ranking.getTurmaVencedora() << '\n';

    return 0;
}
