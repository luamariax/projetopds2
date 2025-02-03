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

void cadastrarAluno(RankingTurmas& ranking) {
    std::string nome, matricula, turma;
    int idade;

    std::cout << "Cadastro de Aluno\n";
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    std::cout << "Idade: ";
    std::cin >> idade;
    std::cin.ignore(); // Para limpar o buffer do teclado
    std::cout << "Matrícula (10 dígitos): ";
    std::getline(std::cin, matricula);
    std::cout << "Turma (deve começar com 'Engenharia'): ";
    std::getline(std::cin, turma);

    try {
        Aluno aluno(idade, nome, matricula, turma);
        std::cout << "Aluno cadastrado com sucesso: " << aluno.getNome() << ", Turma: " << aluno.getTurma() << '\n';
        // Removido o adicionarPontuacao, pois alunos criados não devem adicionar pontos à turma.
    } catch (const std::exception& e) {
        std::cerr << "Erro ao cadastrar aluno: " << e.what() << '\n';
    }
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    RankingTurmas ranking;

    char opcao;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1 - Cadastrar Aluno\n";
        std::cout << "2 - Exibir Ranking\n";
        std::cout << "3 - Adicionar Pontos a uma Turma\n"; // Nova opção para adicionar pontos manualmente
        std::cout << "4 - Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;
        std::cin.ignore(); // Para limpar o buffer do teclado

        switch (opcao) {
            case '1':
                cadastrarAluno(ranking);
                break;
            case '2':
                ranking.exibirRanking();
                std::cout << "Turma vencedora: " << ranking.getTurmaVencedora() << '\n';
                break;
            case '3': {
                std::string turma;
                int pontos;
                std::cout << "Digite o nome da turma: ";
                std::getline(std::cin, turma);
                std::cout << "Digite a quantidade de pontos a adicionar: ";
                std::cin >> pontos;
                std::cin.ignore(); // Limpar o buffer
                ranking.adicionarPontuacao(turma, pontos);
                std::cout << "Pontos adicionados com sucesso!\n";
                break;
            }
            case '4':
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opção inválida! Tente novamente.\n";
        }
    } while (opcao != '4');

    return 0;
}
    return 0;
}
