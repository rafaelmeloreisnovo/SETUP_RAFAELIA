# Guia em 10 minutos — sem conhecimentos de programação

**O SETUP é um manual interativo por arquivos, não um instalador mágico.** Você pode ler tudo com segurança antes de solicitar acesso.

1. Abra o [README](../README.md) e escolha a gaveta (qualidade, credenciais, integração ou evidência).
2. Descubra o **proprietário** e a **conta** certa antes de clicar em configurações.
3. Leia o cartão do [provedor](PROVIDER_SETTINGS.md). Cada cartão explica quem pode alterar, como provar e como reverter.
4. Peça à pessoa autorizada para configurar no **site do provedor**. Não compartilhe uma senha para que outro a configure.
5. Confira o recibo de teste. Sem prova, marque `TOKEN_VAZIO`; com erro, marque `HOLD`.

## Tradutor de nomes
- **Setting:** regra que o serviço aplica, como impedir merge sem revisão.
- **Variable:** informação não confidencial para o programa; exemplo de intenção: `RAFAELIA_SETUP_MODE=PLAN_ONLY` (não criada aqui).
- **Secret:** senha/token/chave; só no cofre seguro do serviço, nunca num arquivo, chat, imagem ou planilha.
- **PAT:** credencial pessoal de API. Só quando indispensável, com prazo e poder mínimo. Preferir GitHub App/OIDC quando cabível.
- **Receipt:** prova com local, data, versão, teste e resultado. **Não inclui o valor da senha.**

### Como reconhecer o resultado
`DECLARED` = desejado; `TOKEN_VAZIO` = desconhecido; `IMPLEMENTED_UNTESTED` = escrito sem teste; `PASS_SCOPED` = teste delimitado passou; `HOLD` = não prosseguir; `NOT_APPLICABLE` = não se aplica, com motivo.

Se surgir dúvida ou pedido de segredo, **não cole a credencial**. Pare e use a rota de suporte do operador autorizado.
