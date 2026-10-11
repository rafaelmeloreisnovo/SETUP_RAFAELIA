# Google Drive — integração de mínimo privilégio

**Drive = corpus, índice, memória documental e recibos. GitHub = código/spec/test/CI do repositório produtor.** Um ID ou URL sozinho não atesta permissão, identidade de arquivo nem bytes.

## Configuração segura
1. Confirmar a **conta Google e a pasta/arquivo** autorizados. Locador completo: `account_alias + file_id + revision + visibility`; contas diferentes não devem ser confundidas.
2. Se houver aplicativo OAuth, justificar cada escopo e finalidade. Preferir `drive.file` por arquivo quando viável. Escopos globais `drive` e `drive.readonly` têm restrições maiores e exigem revisão adequada.
3. Solicitar consentimento no fluxo normal do Google Cloud; nunca transportar OAuth tokens/refresh tokens para o GitHub ou arquivo de recibo.
4. Definir classificação, minimização de dados, retenção, compartilhamento e responsáveis. Em repo público, registrar apenas rota pública/autorizada e prova sem dados pessoais.
5. Fazer **readback** com conta correta, revisão, timestamp e, quando bytes disponíveis, SHA-256 do material realmente visto; senão `TOKEN_VAZIO_HASH`.
6. Divergência → HOLD, guardar as versões e criar sucessor. Não excluir/mover ou sobrescrever arquivos para disfarçar incerteza.

## Rotas canônicas
- [START HERE](https://docs.google.com/document/d/1T4OIN2RC1I-KFsdaQTCqieeZUGYBXissWrdrZMG0DN8/edit)
- [CURRENT_STATE](https://docs.google.com/document/d/1d0J5SkF2S2emBq6jLuTkhYVaSOQiWazJWyRCTMJ-iRI/edit)
- [μWRITE](https://docs.google.com/document/d/1NEU7lg7iUZf1u7lX-SEKuc-knSDGlF5gOx7qzXcJ1yc/edit)

**404 de uma conta não prova exclusão.** Consultar conta de origem, permissão e identidade de arquivo antes de qualquer conclusão.

[Escopos oficiais da API Google Drive](https://developers.google.com/workspace/drive/api/guides/api-specific-auth).
