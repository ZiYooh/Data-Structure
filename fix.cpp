// 201624453 HW02 이진 수식 트리 
#include <iostream>
#include <stack>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <queue>
using namespace std;

// 트리 구성 요소인 노드 구조체 선언
struct Node{
	char value;
	Node* left;
	Node* right;
};

string out;

// 문자가 연산자인지 확인
bool isOperator(char c){
	if (c == '+' || c == '-' || c == '*' || c == '/')
		return true;
	return false;
}

// 문자가 알파벳인지 확인
bool isAlphabet(char c){
	if ('a' <= c && c <= 'z')
		return true;
	return false;
}

// 해당 노드가 외부에 있는 노드인지 확인
bool isExternal(Node* n){
	return(n->left == NULL || n->right == NULL);
}

// infix 표기법으로 출력
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
// 파일에 쓰기 위해 string 리턴형으로 만든 함수
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

// 새로운 노드 생성
Node* newNode(char c){
	Node *temp = new Node;

	temp->value = c;
	temp->left = NULL;
	temp->right = NULL;

	return temp;
};

// char형 배열을 인자로 받아 prefix/postfix 방식 나눠서 2진 수식 트리를 만듬
Node* MakeTree(char exp[]){
	stack<Node *> NodeStack;
	Node *n;
	Node *n1;
	Node *n2;

    // P로 시작할 경우 postfix 방식이다.
	if(exp[0] == 'P'){
		for (int i = 1; i < strlen(exp) - 1; i++)	{
            // 연산자가 아닐 경우
            // 노드 생성 후 스택에 그대로 push 한다
			if (!isOperator(exp[i])){
				n = newNode(exp[i]);
				NodeStack.push(n);
			}
            // 연산자일 경우
            // 노드를 만든 다음에 스택에서 2번 pop 하고
            // pop 해온것들을 child로 만들고 만든 노드를 스택에 push 한다
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

		// 최종적으로 스택에 남은 노드가 root 노드이다
		n = NodeStack.top();
		NodeStack.pop();

		return n;
	}

    // I로 시작할 경우 prefix 방식이다.
	else if(exp[0] == 'I'){
 		for (int i = strlen(exp) - 2; i >= 1; i--) {
            // 연산자가 아닐 경우
            // 노드 생성 후 스택에 그대로 push 한다
    		if (!isOperator(exp[i])) {
				n = newNode(exp[i]);
				NodeStack.push(n);
    		}
            // 연산자일 경우
            // 노드를 만든 다음에 스택에서 2번 pop 하고
            // pop 해온것들을 child로 만들고 만든 노드를 스택에 push 한다
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

		// 최종적으로 스택에 남은 노드가 root 노드이다
	  	n = NodeStack.top();
		NodeStack.pop();

  		return n;
	}

	else{
		cout<<"Error";
		return 0;
	}


}
// 수식이 저장된 배열, 알파벳 저장된 배열, 치환할 숫자가 담긴 배열을 인자로 받는다
int calculate(char exp[], char sub[], double num[]){
	stack<double> NumStack;
	double result;
	double div_result;
	double m1;
	double m2;
    // postfix 방식
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

			// 알파벳이 나올 경우 치환할 숫자를 찾아 해당 알파벳을 바꿔준다
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

    // prefix 방식
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

            // 알파벳이 나올 경우 치환할 숫자를 찾아 해당 알파벳을 바꿔준다
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

// 메인함수 시작 
int main(){
	// 변수 및 큐 선언
	int T;
    string temp1;
    queue<char> temp_queue;
    queue<char> sub_queue;
    queue<double> num_queue;

    // 파일입출력 준비
	ifstream fin;
	ofstream fout;
	fin.open("fix.in");
	fout.open("fix.out");
	fin >> T;

    getline(fin, temp1);

	for(int k = 0; k < T; k++){
        // 홀수번째줄 테스트
        fin.clear();
        getline(fin, temp1);
        for(int i = 0; i < temp1.capacity(); i++){
            if(temp1[i] != ' ')
                temp_queue.push(temp1[i]);
        }

        // 트리를 만들 배열 수식 생성
        int s = temp_queue.size();
        char line[s];

        for(int i = 0; i < s; i++){
            line[i] = temp_queue.front();
            temp_queue.pop();
        }

        // 트리 만든 후 중위표기법 으로 출력
        Node* rr1 = MakeTree(line);
        inorder(rr1);
        fout << print_inorder(rr1)<<endl;
        out = "";
        cout<<endl;

        // 치환할 알파벳이랑 숫자 각각 큐에 넣음
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


        // 수식 계산을 위해 치환할 문자와 숫자 배열 생성
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

        // 수식 계산 결과 출력
        cout << calculate(line, sub, num) << endl;
        fout << calculate(line, sub, num) << endl;
		cout << "파일 쓰기 완료..." << endl; 
        // 프로그램 종료 전 모든 큐 비우기
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

	//파일 닫음
    fin.close();

	return 0;
}
