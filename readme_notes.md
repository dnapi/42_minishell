git branch branch_name
git checkout branch_name
git add filename
git commit -m "Your commit message here"
git push --set-upstream origin branch_name


Precendence of operators:

1. **Parentheses**: Commands and expressions within parentheses `()` are evaluated first.
   
2. **Brace Expansion**: Brace expansion `{}` is performed next.

3. **Tilde Expansion**: Tilde expansion `~` is performed.

4. **Parameter Expansion**: Parameter expansion `${}` is performed.

5. **Command Substitution**: Command substitution `$()` and backtick substitution ```` are performed.

6. **Arithmetic Expansion**: Arithmetic expansion `$((...))` is performed.

7. **Word Splitting**: Word splitting on whitespace is performed after expansions.

8. **Double Quotes**: Double quotes `" "` preserve spaces and treat most special characters literally, except for variables and command substitutions.

9. **Single Quotes**: Single quotes `' '` treat everything literally, including variables and command substitutions.

10. **Filename Expansion (Globbing)**: Filename expansion (also known as globbing or wildcard expansion) `*`, `?`, and `[]` is performed.

11. **Logical OR**: `||` performs a logical OR operation.

12. **Logical AND**: `&&` performs a logical AND operation.

13. **Pipe**: `|` performs piping between commands.

14. **Semicolon**: `;` separates multiple commands on the same line.

15. **Redirection Operators**: Redirection operators `<`, `>`, `>>`, `<<`, and their variants are evaluated next.

16. **Compound Commands**: Compound commands such as `if`, `for`, `while`, and `until` have their own precedence rules within their respective constructs.

Metacharacters in bash 
- **Command Separation**:
  - `;`: Separates multiple commands on the same line.

- **Control Operators**:
  - `&&`: Logical AND operator.
  - `||`: Logical OR operator.

- **Wildcard**:
  - `*`: Matches zero or more characters in filename expansion (globbing).
  - `?`: Matches any single character in filename expansion.
  - `[]`: Matches any one of the enclosed characters in filename expansion.

- **Escape Character**:
  - `\`: Escapes the next character, treating it as a literal character rather than a metacharacter.

- **Special Variables**:
  - `$`: Indicates the beginning of a variable name or special variable.
  - `${}`: Encloses variable names in complex expressions or ensures variable names are interpreted correctly.

- **Expression**:
  - `(( ))`: Evaluate contents as a mathematical expression.
  - `[[ ]]`: Test expression, returning 0 or 1.

- **Redirection**:
  - `>`: Overwrite existing content.
  - `<`: Overwrite existing content.
  - `>>`: Append to existing content.
  - `<<`: Append to existing content.

- **Wildcard**:
  - `?`: Match exactly one of any character.
  - `*`: Match zero or more of any character.

- **Character Classes**:
  - `[0-9]`: Any digit.
  - `[a-z]`: Any lowercase alpha.
  - `[A-Z]`: Any uppercase alpha.
  - `[a-zA-Z]`: Any lowercase or uppercase alpha.
  - `[abc]`: Only a, b, and c.
  - `[!a-z]`: No lowercase alpha.
  - `[!1-3]`: No 1, 2, or 3, but all other digits.
  - `[b-hot]`: Lowercase b through h, and the letters o and t.
  - `[A-M0-9]`: Uppercase alpha A through M, and any digit.

- **Escape Sequences**:
  - `\b`: Backspace.
  - `\e`: Escape.
  - `\f`: Form feed (like a non-returning newline).
  - `\n`: Newline.
  - `\r`: Carriage return.
  - `\t`: Horizontal tab.
  - `\v`: Vertical tab.
  - `\\`: Backslash.
  - `\cH`: Control-H.
  - `\uHHHH`: Unicode character of hexadecimal value HHHH.
  - `\NNN`: 8-bit character with octal value NNN.

