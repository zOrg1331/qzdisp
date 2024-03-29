/*
  Copyright (C) 2004, 2005 Ingo Berg

  Permission is hereby granted, free of charge, to any person obtaining a copy of this
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify,
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef MU_PARSER_ERROR_H
#define MU_PARSER_ERROR_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "muParserDef.h"


namespace mu
{

/** \brief Error codes. */
enum EErrorCodes
{
  // Formula syntax errors
  ecUNEXPECTED_OPERATOR = 0,  ///< Unexpected binary operator found
  ecUNASSIGNABLE_TOKEN  = 1,  ///< Token cant be identified.
  ecUNEXPECTED_EOF      = 2,  ///< Unexpected end of formula. (Example: "2+sin(")
  ecUNEXPECTED_COMMA    = 3,  ///< An unexpected comma has been found. (Example: "1,23")
  ecUNEXPECTED_ARG      = 4,  ///< An unexpected argument has been found
  ecUNEXPECTED_VAL      = 5,  ///< An unexpected value token has been found
  ecUNEXPECTED_VAR      = 6,  ///< An unexpected variable token has been found
  ecUNEXPECTED_PARENS   = 7,  ///< Unexpected Parenthesis, opening or closing
  ecUNEXPECTED_STR      = 8,  ///< A string has been found at an inapropriate position
  ecSTRING_EXPECTED     = 9,  ///< A string function has been called with a different type of argument
  ecVAL_EXPECTED        = 10, ///< A numerical function has been called with a non value type of argument
  ecMISSING_PARENS      = 11, ///< Missing parens. (Example: "3*sin(3")
  ecUNEXPECTED_FUN      = 12, ///< Unexpected function found. (Example: "sin(8)cos(9)")
  ecUNTERMINATED_STRING = 13, ///< unterminated string constant. (Example: "3*valueof("hello)")
  ecTOO_MANY_PARAMS     = 14, ///< Too many function parameters
  ecTOO_FEW_PARAMS      = 15, ///< Too few function parameters. (Example: "ite(1<2,2)")
  ecOPRT_TYPE_CONFLICT  = 16, ///< binary operators may only be applied to value items of the same type
  ecSTR_RESULT          = 17, ///< result is a string

  // Invalid Parser input Parameters
  ecINVALID_NAME        = 18, ///< Invalid function, variable or constant name.
  ecBUILTIN_OVERLOAD    = 19, ///< Trying to overload builtin operator
  ecINVALID_FUN_PTR     = 20, ///< Invalid callback function pointer
  ecINVALID_VAR_PTR     = 21, ///< Invalid variable pointer

  ecNAME_CONFLICT       = 22, ///< Name conflict
  ecOPT_PRI             = 23, ///< Invalid operator priority
  //
  ecDOMAIN_ERROR        = 24, ///< catch division by zero, sqrt(-1), log(0) (currently unused)
  ecDIV_BY_ZERO         = 25, ///< Division by zero (currently unused)
  ecGENERIC             = 26, ///< Generic error

  // internal errors
  ecINTERNAL_ERROR      = 27, ///< Internal error of any kind.

  // The last two are special entries
  ecCOUNT,                    ///< This is no error code, It just stores just the total number of error codes
  ecUNDEFINED           = -1  ///< Undefined message, placeholder to detect unassigned error messages
};

//---------------------------------------------------------------------------
class ParserErrorMsg
{
public:
    typedef ParserErrorMsg self_type;

    ParserErrorMsg& operator=(const ParserErrorMsg &);
    ParserErrorMsg(const ParserErrorMsg&);
    ParserErrorMsg();

   ~ParserErrorMsg();

    static const ParserErrorMsg& Instance();
    string_type operator[](unsigned a_iIdx) const;

private:
    std::vector<string_type>  m_vErrMsg;
    static const self_type m_Instance;
};

//---------------------------------------------------------------------------
/** \brief Error class of the parser.

  Part of the math parser package.

  \author Ingo Berg
*/
/* final */ class ParserError
{
private:
    //------------------------------------------------------------------------------
    /** \brief Replace all ocuurences of a substring with another string. */
    void ReplaceSubString( string_type &strSource,
			   const string_type &strFind,
			   const string_type &strReplaceWith);
    void Reset();

public:
    ParserError();
    explicit ParserError(EErrorCodes a_iErrc);
    explicit ParserError(const string_type &sMsg);
    ParserError( EErrorCodes a_iErrc,
		 const string_type &sTok,
		 const string_type &sFormula = string_type("(formula is not available)"),
		 int a_iPos = -1);
    ParserError( EErrorCodes a_iErrc,
		 int a_iPos,
		 const string_type &sTok);
    ParserError( const char_type *a_szMsg,
		 int a_iPos = -1,
		 const string_type &sTok = string_type());
    ParserError(const ParserError &a_Obj);
    ParserError& operator=(const ParserError &a_Obj);
   ~ParserError();

    void SetFormula(const string_type &a_strFormula);
    const string_type& GetExpr() const;
    const string_type& GetMsg() const;
    std::size_t GetPos() const;
    const string_type& GetToken() const;
    EErrorCodes GetCode() const;

private:
    string_type m_strMsg;     ///< The message string
    string_type m_strFormula; ///< Formula string
    string_type m_strTok;     ///< Token related with the error
    int m_iPos;               ///< Formula position related to the error
    EErrorCodes m_iErrc;      ///< Error code
    const ParserErrorMsg &m_ErrMsg;
};

} // namespace mu

#endif
