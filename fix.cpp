// 201624453 HW02 Binary expression tree
#include <iostream>
#include <stack>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <queue>
using namespace std;

// Node Structure
struct Node{
	char value;
	Node* left;
	Node* right;
};

string out;

// Check for operator
bool isOperator(char c){
	if (c == '+' || c == '-' || c == '*' || c == '/')
		return true;
	return false;
}

// Check for alphabet
bool isAlphabet(char c){
	if ('a' <= c && c <= 'z')
		return true;
	return false;
}

// Check for external node
bool isExternal(Node* n){
	return(n->left == NULL || n->right == NULL);
}

// infix
void inorder(Node *n){
	if(n){
		if(!isExternal(n)){
			cout<<"(";
			inorder(n->left);
		}
		else
			inorder(n->left);
			cout<<n->value;
		if(!isExternal(n)){
			inorder(n->right);
			cout<<")";
		}
		else
			inorder(n->right);

	}
}
// Return string for file output
string print_inorder(Node *n){
	if(n){
		if(!isExternal(n)){
			out += "(";
			print_inorder(n->left);
		}
		else
			print_inorder(n->left);
			out += n->value;
		if(!isExternal(n)){
			print_inorder(n->right);
			out += ")";
		}
		else
			print_inorder(n->right);
	}

	return out;
}

// Make new node
Node* newNode(char c){
	Node *temp = new Node;

	temp->value = c;
	temp->left = NULL;
	temp->right = NULL;

	return temp;
};

// Make binary expression tree by char array
Node* MakeTree(char exp[]){
	stack<Node *> NodeStack;
	Node *n;
	Node *n1;
	Node *n2;

    // Postfix (If first char is 'P')
	if(exp[0] == 'P'){
		for (int i = 1; i < strlen(exp) - 1; i++)	{
			if (!isOperator(exp[i])){
				n = newNode(exp[i]);
				NodeStack.push(n);
			}
			else{
				n = newNode(exp[i]);

				n1 = NodeStack.top();
				NodeStack.pop();
				n2 = NodeStack.top();
				NodeStack.pop();

				n->right = n1;
				n->left = n2;

				NodeStack.push(n);
			}
		}

		n = NodeStack.top();
		NodeStack.pop();

		return n;
	}

    // Infix (If first char is 'I')
	else if(exp[0] == 'I'){
 		for (int i = strlen(exp) - 2; i >= 1; i--) {
    		if (!isOperator(exp[i])) {
				n = newNode(exp[i]);
				NodeStack.push(n);
    		}
		    else {
      			n = newNode(exp[i]);
				n1 = NodeStack.top();
				NodeStack.pop();
				n2 = NodeStack.top();
				NodeStack.pop();

				n->left = n1;
				n->right = n2;

				NodeStack.push(n);
    		}
	  	}

	  	n = NodeStack.top();
		NodeStack.pop();

  		return n;
	}

	else{
		cout<<"Error";
		return 0;
	}


}
// Calculation for binary expression tree
int calculate(char exp[], char sub[], double num[]){
	stack<double> NumStack;
	double result;
	double div_result;
	double m1;
	double m2;
    // postfix
	if(exp[0] == 'P'){
		for (int i = 1; i < strlen(exp) - 1; i++)	{
			if(isOperator(exp[i])){
				if (exp[i] == '+'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m1 + m2;
					NumStack.push(result);
				}

				else if (exp[i] == '-'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m2 - m1;
					NumStack.push(result);
				}

				else if (exp[i] == '*'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m1 * m2;
					NumStack.push(result);

				}

				else if (exp[i] == '/'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m1 / m2;
					NumStack.push(result);
				}

			}

			else if(isAlphabet(exp[i])){
				for(int j = 0; j < strlen(sub); j++){
					if(exp[i] == sub[j]){
						NumStack.push(num[j]);
					}
				}
			}
			else{
				result = (int)exp[i] - 48;
				NumStack.push(result);
			}
		}
		result = NumStack.top();
		NumStack.pop();

		return result;
	}

    // prefix
	else if(exp[0] == 'I'){
 		for (int i = strlen(exp) - 2; i >= 1; i--) {
			if(isOperator(exp[i])){
				if (exp[i] == '+'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m1 + m2;
					NumStack.push(result);
				}

				else if (exp[i] == '-'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m1 - m2;
					NumStack.push(result);
				}

				else if (exp[i] == '*'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

					result = m1 * m2;
					NumStack.push(result);
				}

				else if (exp[i] == '/'){
					m1 = NumStack.top();
					NumStack.pop();
					m2 = NumStack.top();
					NumStack.pop();

                    result = m1 / m2;
					NumStack.push(result);
				}

			}

			else if(isAlphabet(exp[i])){
				for(int j = 0; j < strlen(sub); j++){
					if(exp[i] == sub[j]){
						NumStack.push(num[j]);
					}
				}
			}
			else{
				result = (int)exp[i] - 48;
				NumStack.push(result);
			}
		}
		result = NumStack.top();
		NumStack.pop();

		return round(result);
	}

	else{
		cout<<"Error";
		return 0;
	}


}

int main(){
	int T;
    string temp1;
    queue<char> temp_queue;
    queue<char> sub_queue;
    queue<double> num_queue;

	ifstream fin;
	ofstream fout;
	fin.open("fix.in");
	fout.open("fix.out");
	fin >> T;

    getline(fin, temp1);

	for(int k = 0; k < T; k++){
        fin.clear();
        getline(fin, temp1);
        for(int i = 0; i < temp1.capacity(); i++){
            if(temp1[i] != ' ')
                temp_queue.push(temp1[i]);
        }

        int s = temp_queue.size();
        char line[s];

        for(int i = 0; i < s; i++){
            line[i] = temp_queue.front();
            temp_queue.pop();
        }

        Node* rr1 = MakeTree(line);
        inorder(rr1);
        fout << print_inorder(rr1)<<endl;
        out = "";
        cout<<endl;

        char a;
        double b;
        while(true){
            fin >> a >> b;
            if(a == 'E'){
                break;
            }
                sub_queue.push(a);
                num_queue.push(b);
        }

        int sq = sub_queue.size();
        int nq = num_queue.size();

        char sub[sq];
        double num[nq];


        for(int i = 0; i < sq; i++){
            sub[i] = sub_queue.front();
            sub_queue.pop();

            num[i] = num_queue.front();
            num_queue.pop();
        }

        cout << calculate(line, sub, num) << endl;
        fout << calculate(line, sub, num) << endl;
		cout << "파일 쓰기 완료..." << endl; 

        while(!sub_queue.empty()){
            sub_queue.pop();
        }
        while(!num_queue.empty()){
            num_queue.pop();
        }
        while(!sub_queue.empty()){
            sub_queue.pop();
        }

        getline(fin, temp1);
	}

    fin.close();

	return 0;
}
