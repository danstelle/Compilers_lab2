#include <iostream>
#include "lex.h"
#include "parse.h"
#include "tokens.h"

int yylval = 0;
int token = -1;
int oldToken = -1;

//*******************************************
void Error(std::string expecting)
{
    std::cout << "Found " << yytext << " when expecting a " << expecting;
    std::cout << " in line " << yylineno << std::endl;
}
//*******************************************
int GetToken()
{
    if (token > 0)
    {
        oldToken = token;
        token = -1;
    } else {
        oldToken = yylex();
    }

    return oldToken;
}
//*******************************************
void UnGetToken()
{
    token = oldToken;
}
//*******************************************
bool FindPROG()
{
    bool valid = true;
    
    // If we don't find a STMTS, program is invalid
    if (!FindSTMTS())
        valid = false;
        
    // If we find a STMTS, but we don't find an "end",
    // then the program is invalid
    else
    {
        int token = GetToken(); // Get the next token
        
        // If the token is not an "end", display error
        if (token != END)
        {
            Error("'end'");
            valid = false;
        }
    }
    
    return valid;
}
//******************************************
bool FindSTMTS()
{
    if (!FindSTMT())
    {
        int token = GetToken();
        
        if (token == END)
        {
            UnGetToken();
            return true;
        }
        
        UnGetToken();
        
        if (!FindSTMTS())
            return true;
    }
    else if (!FindSTMTS())
        return false;
        
    return true;
}
//******************************************
bool FindSTMT()
{
    bool valid = true;
    
    // If we don't find an EXPR,
    if (!FindEXPR())
        valid = false;
    
    // If we do find an EXPR
    else
    {
        int token = GetToken(); // Get the next token
        
        // If the token is not a ';',
        // the STMT is invalid
        if (token != ';')
        {
            Error("';'");
            UnGetToken();
            Sync();
        }
        else
            std::cout << "Found a statement\n";
    }
    
    return valid;
}
//******************************************
bool FindEXPR()
{
    bool valid = true;
    int token = GetToken(); // Get the next token
    
    // If the token is not a '(',
    // then look for a TERM
    if (token != '(')
    {
        UnGetToken(); // Return token to input
        
        // If we don't find a TERM
        if (!FindTERM())
            valid = false;
    }
    
    // If we do find a '('
    else
    {
        // If we don't find an EXPR
        if (!FindEXPR())
            valid = false;
        
        // If we do find an EXPR
        else
        {
            token = GetToken(); // Get the next token
            
            // If the token is not a ')',
            // display an error
            if (token != ')')
            {
                Error("')'");
                UnGetToken();
                valid = false;
                Sync();
            }
            
            // If we do find a ')'
            else
            {
                // If we don't find an EXPR_P
                if (!FindEXPR_P())
                    valid = false;
            }
        }
    }
    
    return valid;
}
//******************************************
bool FindEXPR_P()
{
    bool valid = true;
    
    // If we find a PLUSOP
    if (FindPLUSOP())
    {
        int token = GetToken(); // Get the next token
        
        // If the token is not a '(',
        // display an error
        if (token != '(')
        {
            UnGetToken();
            Error("'('");
            valid = false;
            Sync();
        }
        
        // If we do find a '('
        else
        {
            // If we do not find and EXPR,
            if (!FindEXPR())
                valid = false;
            
            // If we do find and EXPR
            else
            {
                token = GetToken(); // Get the next token
                
                // If token is not a ')',
                // display an error
                if (token != ')')
                {
                    UnGetToken();
                    Error("')'");
                    valid = false;
                    Sync();
                }
                
                // If we do find a ')'
                else
                {
                    // If we do not find another EXPR_P,
                    // display an error
                    if (!FindEXPR_P())
                        valid = false;
                }
            }
        }
    }
    
    return valid;
}
//******************************************
bool FindPLUSOP()
{
    bool valid = true;
    int token = GetToken(); // Get the next token
    
    // If the token is a '+' or a '-',
    // return false
    if (token != '+' && token != '-')
    {
        UnGetToken();
        valid = false;
    }
    
    return valid;
}
//******************************************
bool FindTERM()
{
    bool valid = true;
    int token = GetToken(); // Get the next token
    
    // If the token is not a '['
    if (token != '[')
    {
        // Check to see if it is a number
        // If it isn't, display an error
        if (token != INT_VAL)
        {
            UnGetToken();
            //Error("term"); // GETTING AN ERROR HERE
            valid = false;
        }
    }
    
    // If we do find a '['
    else
    {
        // If we don't find an EXPR,
        // Display an error
        if (!FindEXPR())
        {
            //Error("expression");
            valid = false;
        }
        
        // If we do find an EXPR
        else
        {
            token = GetToken(); // Get the next token
            
            // If the token is not a ']',
            // display an error
            if (token != ']')
            {
                UnGetToken();
                Error("']'");
                valid = false;
                Sync();
            }
            
            // If the token is a ']'
            else
            {
                // If we don't find a TERM_P,
                // display an error
                if (!FindTERM_P())
                {
                    //Error("term prime");
                    valid = false;
                }
            }
        }
    }
    
    return valid;
}
//******************************************
bool FindTERM_P()
{
    bool valid = true;
    
    // If we find a TIMESOP
    if (FindTIMESOP())
    {
        int token = GetToken(); // Get the next token
        
        // If we don't find a '['
        if (token != '[')
        {
            UnGetToken();
            Error("'['");
            valid = false;
            Sync();
        }
        
        // If we do find a '['
        else
        {
            // If we don't find an EXPR,
            // display an error
            if (!FindEXPR())
            {
                //Error("expression"); // ERROR HERE
                valid = false;
            }
            
            // If we do find an EXPR
            else
            {
                token = GetToken(); // Get the next token
                
                // If the token is not a ']',
                // display an error
                if (token != ']')
                {
                    UnGetToken();
                    Error("']'");
                    valid = false;
                    Sync();
                }
                
                // If we do find a ']'
                else
                {
                    // If we don't find a TERM_P,
                    // display an error
                    if (!FindTERM_P())
                        valid = false;
                }
            }
        }
    }
    
    return valid;
}
//******************************************
bool FindTIMESOP()
{
    bool valid = true;
    int token = GetToken(); // Get the next token
    
    // If the token is not a '*' or a '/',
    // return false
    if (token != '*' && token != '/')
    {
        UnGetToken();
        valid = false;
    }
    
    return valid;
}
//******************************************
void Sync()
{
    int token = GetToken();
    
    // Loop until next semicolon
    while (token != ';' && token != END)
        token = GetToken();
}