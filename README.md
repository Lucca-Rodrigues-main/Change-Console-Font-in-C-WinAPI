# Change-Console-Font-in-C-WinAPI
A set of functions in C to dynamically alter font settings (size, family, weight, etc.) using WinAPI. Useful for terminal customization or compatibility testing.

---

### **Introdução**
Este código em C permite alterar e restaurar dinamicamente a fonte do console do Windows, podendo ser útil para personalizar a aparência do terminal ou garantir compatibilidade em diferentes sistemas.

A janela do IDE é um ambiente limitado para alterações que afetam o console do Windows, pois muitas configurações de fonte e terminal são controladas pelo próprio IDE ou podem ser sobrescritas durante a execução. Para garantir que as mudanças de fonte sejam aplicadas corretamente, recomenda-se testar o programa diretamente no **Prompt de Comando** ou no **Windows Terminal**, onde as configurações do console são gerenciadas de forma mais direta e estável.

---

### **Funções**
- **`GuardaFonte()`**: Armazena as configurações atuais do console (fonte, codificação, modo de exibição, etc.) em uma estrutura `FONTE`.
- **`AlteraFonte()`**: Aplica uma nova configuração de fonte com base nos parâmetros fornecidos.
- **`RestauraFonte()`**: Volta às configurações originais salvas por `GuardaFonte()`.

---

### **Como Usar**
1. Chame `GuardaFonte()` para salvar a configuração atual.
2. Modifique a estrutura `FONTE` com os parâmetros desejados.
3. Use `AlteraFonte()` para aplicar as mudanças.
4. Para restaurar, chame `RestauraFonte()`.

---

### **Exemplo**
Fonte original CP437:

<img width="263" height="177" alt="image" src="https://github.com/user-attachments/assets/99c2b380-4970-46fe-be8a-e95c8aea04dd" />

Alteração para Consolas:

<img width="297" height="291" alt="image" src="https://github.com/user-attachments/assets/373f4490-7004-4699-9325-4793bf87df75" />
