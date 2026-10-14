# RAFAELIA Ω — Federação existente: freestanding, NEON e autoridade (auditoria de fonte)

Data da observação: 2026-10-13. Destino: `SETUP_RAFAELIA`, **PLAN_ONLY**, sem provider-write, alterações de código produtivo, segredos, merge, release ou alegações físicas novas.

## Por que este documento existe

O autor informou implementações pré-existentes em RafPolimata, RafGitTools, Vectras, Termux app/Packages/API, Frida, Mapa, Gaia, Rafaelia Private, RMRCTI/llama, C/ASM/Python/Java, GPU, SIMD NEON, ABI, CI, Android SDK/NDK/ART/DEX/ELF/NUMA, processamento branchless/void, memória-cache, cadeia de custódia e elementos B1–B20. **Não reinventar esses componentes, nem inferir sucesso de um README**. Roteamento canônico: START HERE → CURRENT_STATE → SETUP (capacidade/política) → **produtor de código** → teste no HEAD → receipt → decisão.

`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`
`TOKEN_VAZIO != 0`
`IMPLEMENTED_UNTESTED != PASS`

## Amostragem GitHub: identidade de source branch/HEAD

A leitura contém 93 repositórios acessíveis ao conector GitHub do titular; esta tabela **não** audita todos os arquivos de todos eles. Branch SHA completa observada e exemplo de rota:

| Produtor | HEAD observado | Evidência mínima localizada | Limite do claim |
|---|---|---|---|
| SETUP_RAFAELIA | main `d67b6ebb81398d97fa2d2cae4016905fa0ddd519` | config/setup-catalog.v1.json; gap-register.v1.json (18 controles); tools/validate_setup.py; .github/workflows/setup-plan-validation.yml | Plano offline apenas, não habilita policies no GitHub |
| RafPolimata | main `8d0fb29904b0bbd990b8fec08c57e5ab05db42dd` | freestanding/include/raf_fs_numa_plan.h; docs/architecture/FREESTANDING_ELF_DEX_NUMA_V1.md; arquivos C hardware no root | NUMA é plano com topologia recebida, ELF/DEX são parsers estruturais limitados, não implantação NUMA/ART/runtime |
| RafGitTools | main `5cf98f638cb9a32956576f6e88c8e83ee8467822` | COMPILER/README_RAFAELIA_ROOT_OPTIMIZER.md: C-like → ARM64/x86-64, subconjunto branchless, AST/IR com rastreio | Não é compilador C completo nem prova execução equivalente em todos os alvos |
| Vectras-VM-Android | master `6e4d99c8d90a3a1ddcb68933c693815d7c6e847b` | core/, android/, app/, CMakeLists, AUTHORSHIP_CLEANROOM_PLAN.md | ABI, runtime de VM, licenças e compatibilidade não reexecutadas nesta auditoria |
| Mapa | main `3a38e9f6fc7dd04169d13c42a1f274a4530fdc31` | atlas/, architecture/, control-plane/, docs/, audits/, TERMUX_ECOSYSTEM_TOKEN_VAZIO_UNIFIED_MAP.md | Mapa/índice não equivale a dispositivo ou enforcement executado |
| termux-app-rafacodephi | master `d56d18e57dc2a358d8fb215a0e8687fce18b8ee0` | Makefile.freestanding; BOOTSTRAP_LOWLEVEL_RAFAELIA.txt; install e contract docs | Hosted Android e códigos upstream devem ser distinguidos de módulos autorais |
| termux-packages | main `8ba1b3bdc6d4a86998612ef26a7d52b3bd1b460d` | core/, ndk-patches/, profiles/, packages/, scripts/, THIRD_PARTY_PROVENANCE.md | Receitas de build têm dependências upstream; sem claim de toolchain 100% autoral |
| termux-api_rafcodephi | master `6e1156b58e05f6286af62724e7744407387cf4e4` | native/, app/, ci/, tests/, THIRD_PARTY_PROVENANCE.md | JNI/API Android plataforma hospedada; core pode ser separado |
| frida-desktop | main `6747df6b7dfac1b4685a247be940c283d017b27a` | android/, modules/, subprojects/, tests/, THIRD_PARTY_PROVENANCE.md | Upstream Frida e derivados não são integralmente novos/originais |
| GAIA_phi | main `264a698756b81bb952fa2b0e4bf80b4d59720d35` | FIBER-H-CTX.c, benchmarks e docs; relações Gaia em múltiplos repositórios | Execução física e licença por submódulo não revalidadas |
| GaiaPhiRafcode | main `7a0e50f36b2e2c984f1d850e87e8282ec8468f0e` | raf_t7_neon_batch4.c, raf_t7_core.S, RAFAELIA_T7_ARM32_NEON_BATCH4_REPORT.md, neon_proof.txt | Benchmark ARM32 NEON histórico, não uma medição fresca; executável reportado usa libc/Android linker |
| llamaRafaelia / RMRCTI | master `b532b7678e4f86b1b5dccf5f54a6b310f9014b34` | rmrCti/INDEX.md, omega_layersbit.c, omega_threads*, pmep_generate.py; 20 frames_seed | 17/20 frames com texto no relato, três placeholders; conjunto não representa garantia de corpus completo |

**Outros alvos localizados, não periciados integralmente:** Rafaelia_Private (arquitetura HD_CACHE Python e dados), GAIA-PDS-PHI (repositório mínimo), Rafaelia_Core, llvm-project_rafcode, android_frameworks_base_rafaelia, qemu_rafaelia, Rafcodephi_Sdk_ndkJni_c_py_sh_asm_lua_rs_go_swift_perl_yml_. Identidade proprietária de toda implementação de upstream não pode ser presumida.

## NEON 128 × 4096 — separar unidades e proveniência

Documento-fonte do Drive [RAFAELIA — NEON 4096, 2026-08-29](https://docs.google.com/document/d/1FbQtJFjooUjkQzg_1DOaKR87KEMXpH2XjvH_1kXuIzA/edit) diferencia:

- Uma instrução NEON Q utiliza vetor de **128 bits = 16 bytes**. Para `u32`, são 4 lanes por Q; `batch4` da T7 tem 7 canais × 4 lanes = 28 valores ou 112 bytes sem temporários.
- AArch64 expõe 32 registradores vetoriais de 128 bits: 4096 **bits** = 512 **bytes**, capacidade do banco, não tamanho do buffer externo.
- Um núcleo ARMv7 recuperado reservava **4096 bytes = 4 KiB** de buffer na pilha. Em blocos de 32 bytes, capacidade teórica de **128 blocos**. 512 passadas de 128 blocos para 65536 iterações são apenas uma conta condicional: não foi mostrado avanço do ponteiro no trecho recuperado. `full_sweep=NOT_PROVEN`, `pointer_traversal=OPEN`.
- Estes 4096 bytes não são os 4096 bits do banco registrador AArch64. Dizer “128 NEON resultando 4096, e não 512” exige enunciar **qual grandeza**: memória externa em bytes ou capacidade de registradores em bits.
- Código real em [GaiaPhiRafcode/raf_t7_neon_batch4.c](https://github.com/rafaelmeloreisnovo/GaiaPhiRafcode/blob/main/raf_t7_neon_batch4.c) inclui `stdint.h`, `stdio.h`, `time.h`, `arm_neon.h`. Usa intrinsics NEON no kernel, mas o `main` de benchmark usa tempo/printf do runtime; ELF32 PIE e loader Android constam no relatório. Portanto **não declarar o executável inteiro freestanding**; separar núcleo num header/objeto puro e harness hosted. Em relatório histórico, FNV64 escalar=NEON `1fb850b3965b77e7` e speedup 2.55×–3.91×, sem reexecutar hardware nesta auditoria.
- `raf_t7_core.S` na mesma raiz é **SysV x86_64**, apesar de o repositório conter NEON ARM32 em outros arquivos. Não atribuir ISA ARM ao assembly x86_64.

## Provedores — branch `protected` não equivale a Zero Trust efetivo

Consultas REST ao vivo na data da observação:

| Repositório | default branch .protected | Ruleset GET | Interpretação |
|---|---|---|---|
| SETUP_RAFAELIA | false | [] | Não há proteção efetiva demonstrada |
| RafPolimata | true | #17659629 active | Regras observadas: deletion, non_fast_forward; `ref_name.include=[]`. Não certifica revisão independente ou build exato obrigatório |
| RafGitTools | true | [] | Possível classic branch protection; detalhes não verificados. Não inferir ausência nem existência de revisores |
| termux-app-rafacodephi | true | #21908888 active | `~DEFAULT_BRANCH`, PR rule com `required_approving_review_count=0`, `require_code_owner_review=true`, 5 atores bypass always. Não equivale a revisão distinta garantida |
| Mapa | false | #21909304 disabled | Rule explícita desativada; sem enforcement |
| Vectras-VM-Android | false | HTTP 403 (plano/permissão) | Não transformar incapacidade de ler rulesets em prova de ausência |
| termux-packages | false | [] | Sem proteção observada |
| termux-api_rafcodephi | false | [] | Sem proteção observada |
| frida-desktop | false | [] | Sem proteção observada |
| GAIA_phi / GaiaPhiRafcode / llamaRafaelia | false | HTTP 403 | Capacidade/política de provedor não auditada, separar ausência do dado |

`SETUP_RAFAELIA` tem 18 controles em `config/gap-register.v1.json`, com `state` tipado, `cause`, fonte e falsificador. Sua CI offline passa/fracassa sobre contrato de plano, mas **não ativa proteção, revoga bypass, dá revisão independente ou prova CodeScan**. Não confundir `RUN_COMPLETED` com `ENFORCEMENT_CONFIRMED`.

## Fronteiras multidisciplinares ainda não equivalentes

| Dimensão | Componente ou afirmação observada | Pendência e falsificador |
|---|---|---|
| C/ASM freestanding | RafPolimata L0 NUMA/ELF/DEX, RMRCTI omega_layersbit, núcleos T7 | Object-level `nm -u`, sections, `readelf -d`, ABI exact SHA, portability targets; distinguir `-ffreestanding` de linkagem realmente sem runtime |
| Python/Java | Orquestrador, otimização e caches; Android UI | Sempre hosted onde de fato dependem de intérprete/JVM/ART; adaptar borda, não renomear Python/JVM como bare-metal |
| SDK/NDK/ART/DEX/ELF | SDK/NDK são ferramentas externas de build; ART é runtime Android; ELF/DEX podem ser analisados por parser autoral | Parser estrutural não é loader, linker, class verifier nem ART autoral |
| NUMA e cache | RafPolimata planeja NUMA com snapshot externo; Rafaelia_Private HD cache define tiers L1/L2/L3 **lógicos** em Python | Cache de aplicação não prova controle de caches L1/L2/L3 da CPU; execução com afinidade/páginas necessita recibo físico separado |
| GPU e processador | Componentes/arquivos relevantes distribuídos por Vectras/Android/GAIA | `GPU_FREESTANDING_PHYSICAL=TOKEN_VAZIO`; provar ISA/API, driver, kernel despachado, binary, ciclos e memória de dispositivo |
| Ciclos, branchless e void | RafGitTools otimizador em subconjunto; RMRCTI assinaturas e marcadores void; Gaia bench | Inspecionar assembly produzido, contagem dinâmica de branches/perf (quando viável), sem tratar ausência/VOID como zero |
| Custódia | RMRCTI `pmep_generate.py` com manifestos e hashes; Drive relatório de indexações | Hashes não certificam completude temporal; garantir source generation, SHA, produtor, data, counterexample e readback |
| SNMD, VNI | Siglas citadas, sem localização inequívoca nas buscas focalizadas | `UNRESOLVED_IDENTIFIER`, nunca trocar automaticamente por SIMD, NUMA ou JNI |
| B1–B20 | `llamaRafaelia/rmrCti/INDEX.md` documenta **20 frames_seed**, 17 com texto e 3 placeholders; RafPolimata tem outra notação `B20` de base vigesimal | **Não foi demonstrado** que “B1–B20” corresponde a um desses conjuntos. Exigir source path/manifest semanticamente inequívoco, sem deduplicar, sem preencher desconhecidos como zero |

## Falsificadores de integração recomendados (não executados aqui)

1. Inventariar todos os núcleos `CORE_FREESTANDING` versus `PLATFORM_ADAPTER` e `BUILD_TIME_TOOL` usando objeto/ABI exatos e proveniência de licença; não copiar código upstream indevidamente.
2. Tratar NEON em quatro recibos: `REGISTER_GEOMETRY`, `PHASE_BUFFER_BYTES`, `POINTER_TRAVERSAL`, `DEVICE_BENCHMARK`. Refutar primeira proposta de “varredura completa” por perturbação do ponteiro e por cobertura de blocos; não alegar 4096 bytes de registrador.
3. Cruzar `SETUP` controls com produtores existentes por HEAD e linked evidence, **sem duplicar motores**. Alvos devem ser ampliados em versão tipada sucessora e testada, preservando catálogo V1.
4. Exigir ruleset readback com branch target, required checks, aprovação distinta, bypass audit e **tentativa negativa de merge que o provedor recuse**. Resolver P0 antes da promoção. Se API negar, declarar AUTHORITY/PROVIDER_CAPABILITY=TOKEN_VAZIO.
5. Realizar comparações A/B ARMv7 e AArch64, cache/spill, alinhamento, buffers efetivamente percorridos, MPM, tempo/ciclos e NEON determinismo por exato artefato. GPU executado fica fora de claims até receipt real.
6. Procurar índice primário **B1–B20**, significados próprios de SNMD/VNI e sua relação com `RMRCTI`, Gaia e segurança, sem inferir etimologia.

## R3

`F_ok`: fontes concretas em múltiplos repos, geometria 4096 bits/bytes diferenciada, SETUP 18 controles tipados, constatação de políticas fracas/desativadas, benchmark T7 histórico localizado.

`F_gap`: cobertura completa de 93 repos, ABI/GPUs físicos e dispositivos, exact-HEAD execução de todos os núcleos, direitos de todos os forks, semânticas B1–B20/SNMD/VNI, provider independente.

`F_next`: revisar esta matriz como **plano** no PR, depois fechar as linhas por produtor com source-side falsifier, revisão independente e readback nativo do provedor; não promover/merge.

**Decisão:** `PLAN_ONLY / NO_PROMOTION / NO_DELETION / NO_PROVIDER_WRITE`. Documento não concede autoridade para modificar módulos de terceiros, habilitar configuração ou instalar produção.
