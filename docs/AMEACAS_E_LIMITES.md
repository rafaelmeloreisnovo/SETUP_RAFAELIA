# Privacidade, ameaça e Zero Trust

| Risco | Controle | Prova | Failsafe |
| --- | --- | --- | --- |
| Credencial publicada | cofre do provider; sem valor no git | diff e secret scan | revogar/rotacionar, preservar recibo sem valor |
| Autorrevisão | aprovação de pessoa distinta | reviews API + SHA | HOLD |
| Merge indevido | ruleset ATIVO + teste negativo | GET do provider + rejeição segura | HOLD |
| Bypass oculto | inventário dos actors | identities + ruleset readback | revisar redução do escopo |
| Drive amplo | escopo OAuth restrito por finalidade | consentimento + conta + file ID | revogar/restringir |
| Artefato falso | fonte SHA+hash real+assinatura | manifest+attestation | quarentena e rollback |
| PR verde enganoso | falhas por domínio separadas | logs exatos + falsificador | não promover |
| Exclusão indevida | armazenamento append-only | predecessor + versão | restaurar com aprovação |

**Zero Trust:** identidade, permissão, contexto, finalidade, tempo, fonte, dado, execução e recibo são verificações independentes. Não inferir que uma chave declarada existe; não inferir que settings em JSON foram aplicadas.

**Limites do SETUP v1:** sem acesso administrativo de gravação em GitHub rulesets/secrets, sem autorização para Google OAuth, sem auditoria integral das contas, sem execução física, sem garantia de conformidade jurídica ou certificação ISO/Six Sigma. Tudo isso permanece `TOKEN_VAZIO/HOLD` até prova nativa.
