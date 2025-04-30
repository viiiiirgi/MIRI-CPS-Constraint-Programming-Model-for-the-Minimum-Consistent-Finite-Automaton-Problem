#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace Gecode;
using namespace std;

struct InputData {
    vector<vector<int>> accept_sequences;
    vector<vector<int>> reject_sequences;
    string input_copy;
};

class MCFA : public Space {
protected:
    int n;
    IntVarArray transitions;
    BoolVarArray accepting;
    const InputData& data;

public:
    MCFA(int n_states, const InputData& input_data) :
        n(n_states),
        transitions(*this, n * 2, 0, n - 1),
        accepting(*this, n, 0, 1),
        data(input_data) {

        for (const auto& seq : data.accept_sequences) {
            IntVar currentState(*this, 0, n - 1);
            rel(*this, currentState, IRT_EQ, 0);

            for (int bit : seq) {
                IntVar nextState(*this, 0, n - 1);
                IntVar index = expr(*this, currentState * 2 + bit);
                element(*this, transitions, index, nextState);
                currentState = nextState;
            }

            
            for (int i = 0; i < n; ++i) {
                rel(*this, (currentState == i) >> (accepting[i] == 1));
            }
        }

        for (const auto& seq : data.reject_sequences) {
            IntVar currentState(*this, 0, n - 1);
            rel(*this, currentState, IRT_EQ, 0);

            for (int bit : seq) {
                IntVar nextState(*this, 0, n - 1);
                IntVar index = expr(*this, currentState * 2 + bit);
                element(*this, transitions, index, nextState);
                currentState = nextState;
            }

            
            for (int i = 0; i < n; ++i) {
                rel(*this, (currentState == i) >> (accepting[i] == 0));
            }
        }

        for (int i = 0; i < n; ++i) {
            rel(*this, transitions[i * 2] <= transitions[i * 2 + 1]);
        }

        branch(*this, transitions, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
        branch(*this, accepting, BOOL_VAR_NONE(), BOOL_VAL_MIN());
    }

    MCFA(MCFA& s) : Space(s), n(s.n), data(s.data) {
        transitions.update(*this, s.transitions);
        accepting.update(*this, s.accepting);
    }

    virtual Space* copy() {
        return new MCFA(*this);
    }

    void print(ostream& os) const {
        os << data.input_copy << endl << endl;
        os << n << endl;
        for (int i = 0; i < n; ++i) {
            os << transitions[i * 2 + 0].val() << " "
               << transitions[i * 2 + 1].val() << " "
               << accepting[i].val() << endl;
        }
    }
};

int main() {
    try {
        InputData data;
        stringstream input_buffer;
        string line;

        int num_accept, num_reject;
        getline(cin, line);
        input_buffer << line << endl;
        stringstream(line) >> num_accept;

        for (int i = 0; i < num_accept; ++i) {
            getline(cin, line);
            input_buffer << line << endl;
            stringstream ss(line);
            int len;
            ss >> len;
            vector<int> seq(len);
            for (int& bit : seq) ss >> bit;
            data.accept_sequences.push_back(seq);
        }

        getline(cin, line);
        input_buffer << line << endl;
        if (line.empty()) {
            getline(cin, line);
            input_buffer << line << endl;
        }

        stringstream(line) >> num_reject;
        for (int i = 0; i < num_reject; ++i) {
            getline(cin, line);
            input_buffer << line << endl;
            stringstream ss(line);
            int len;
            ss >> len;
            vector<int> seq(len);
            for (int& bit : seq) ss >> bit;
            data.reject_sequences.push_back(seq);
        }

        data.input_copy = input_buffer.str();
        if (!data.input_copy.empty() && data.input_copy.back() == '\n') {
            data.input_copy.pop_back();
        }

        for (int n = 1; n <= 10; ++n) {
            MCFA* model = new MCFA(n, data);
            DFS<MCFA> engine(model);
            MCFA* solution = engine.next();
            delete model;
            if (solution) {
                solution->print(cout);
                delete solution;
                return 0;
            }
        }
        return 1;
    }
    catch (Exception& e) {
        cerr << "Gecode exception: " << e.what() << endl;
        return 1;
    }
    catch (exception& e) {
        cerr << "Standard exception: " << e.what() << endl;
        return 1;
    }
}
