#include "Expression.h"

Expression::Expression()
{
    //ctor
}

	stack<char> operators ;
	stack<string> operands ;
	string expresion ;
	int counter;

	public static void main(String[] args) {
		App prog = new App();
		prog.scanEXP();
	}

int Expression::scanEXP(string expr)
{
    string postFixExpresion = "";
    //System.out.print("enter the expresion with only variable form : ");
    expresion = expr ;
    expresion = putOnForm(expresion);
    postFixExpresion = postFix();
}

string Expression::postFix (){
		StringTokenizer cutter = new StringTokenizer(expresion," ");
		StringBuilder postfix = new StringBuilder();
		counter = 0;


		while(cutter.hasMoreElements()){

			char symb = cutter.nextToken().charAt(0);

			if(symb >= 97 && symb <= 122){
				counter++;
				postfix.append(symb+" ");
			}
			else{

				if(symb == ')' ){

					while(operators.peek().value != '('){
						postfix.append(operators.pop().value+" ");
					}
					operators.pop();

				}

				else if(symb == '(' ){
					ListNode newNode = new ListNode(symb);
					operators.push(newNode);
				}

				else if(symb == '+' || symb == '-'){

					boolean check = true;

					while(check){
						char element = 0;
						if(! operators.isEmpty()){
							element = operators.peek().value;
						}

						if(operators.isEmpty() || element == '('){

							ListNode newNode = new ListNode(symb);
							operators.push(newNode);
							check=false;
						}

						else {
							postfix.append(operators.pop().value+" ");
						}

					}
				}

				else if(symb == '*' || symb == '/'){

					boolean check = true;

					while(check){
						char element = 0;
						if(! operators.isEmpty()){
							element = operators.peek().value;
						}

						if(operators.isEmpty() || element == '(' || element == '+' || element == '-'){

							ListNode newNode = new ListNode(symb);
							operators.push(newNode);
							check=false;
						}

						else {
							postfix.append(operators.pop().value+" ");
						}

					}
				}
			}
		}

		while(! operators.isEmpty()){
			postfix.append(operators.pop().value+" ");
		}

		String expresionPostfix =  postfix.toString();
		return expresionPostfix;
	}

	public double InFix (String exp){

		int i;
		int[] values = new int[counter];
		double result =0 ;

		for( i = 0 ;  i< counter ; i++){

			System.out.println("enter the value of variable number "+(i+1)+" :");
			values[i] = reader.nextInt();
		}
		StringTokenizer cutter = new StringTokenizer(exp," ");

		i=0;
		while(cutter.hasMoreElements()){

			char symb = cutter.nextToken().charAt(0);

			if(symb >= 97 && symb <= 122){
				String value = Integer.toString(values[i]);
				ListNode1 newNode = new ListNode1(value);
				operands.push(newNode);
				i++;
			}
			else{

				if(symb == '+'){
					double operand1 = Double.parseDouble(operands.pop().value);
					double operand2 = Double.parseDouble(operands.pop().value);
					result = (operand2 + operand1);
					String value = Double.toString(result);
					ListNode1 newNode = new ListNode1(value);
					operands.push(newNode);
					}

				else if(symb == '-'){
					double operand1 = Double.parseDouble(operands.pop().value);
					double operand2 = Double.parseDouble(operands.pop().value);
					result = (operand2 - operand1);
					String value = Double.toString(result);
					ListNode1 newNode = new ListNode1(value);
					operands.push(newNode);
					}

				else if(symb == '*'){
					double operand1 = Double.parseDouble(operands.pop().value);
					double operand2 = Double.parseDouble(operands.pop().value);
					result = (operand2 * operand1);
					String value = Double.toString(result);
					ListNode1 newNode = new ListNode1(value);
					operands.push(newNode);
					}

				else if(symb == '-'){
					double operand1 = Double.parseDouble(operands.pop().value);
					double operand2 = Double.parseDouble(operands.pop().value);
					result = (operand2 / operand1);
					String value = Double.toString(result);
					ListNode1 newNode = new ListNode1(value);
					operands.push(newNode);
					}

				}
			}
		return result;
	}

string Expression::putOnForm(string s){
		string expr = "" ;
		int i;
		int stringLength , stringStart=0 ;
		for(i=0 ; i < s.length() ; i++){
            if(s[i]=='(' || s[i]==')' || s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/' )
            {
                stringLength = i - stringStart ;
                expr += s.substr( stringStart , stringLength ) + " " +  s.substr(i,1) + " " ;
                if(i!=s.length()-1)
                    stringStart = i+1 ;
            }
        }
		return expr ;

	}
