# RAFAELIA APK Authority — candidate standalone repository

**Status:** SOURCE_CANDIDATE / NOT_A_RELEASE / NO_MERGE. Source boundary: \`SETUP_RAFAELIA\` incubator; intended producer: future independent \`RAFAELIA-APK-AUTHORITY\` repository. Existing \`apk-guardian-rafaelia\` remains the static inspection consumer, not a signing authority.

## Purpose

Versioned, dependency-free-at-Python-package-level *host-side* release inventory and SHA-256 integrity probe for APK/AAB/compiler/ELF artifacts, an auditable signing policy and exact-head falsification tests. This is **not** an Android APK signer, not a freestanding compiler, not a Play Console connector, not legal registration of authorship, and not SLSA certification.

Boundary: \`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM\`; \`IMPLEMENTED_UNTESTED != PASS\`; \`TOKEN_VAZIO != 0\`.

## Dependency ledger

| Layer | Present | Boundary |
| --- | --- | --- |
| Catalog/verify-hash | Python 3 stdlib only | HOSTED, no third-party pip packages |
| SHA-256 | stdlib hashlib | Host implementation, NOT freestanding C |
| Cryptographic signing/verification | NOT_IMPLEMENTED | release approval blocked |
| Native Android APK/AAB build | NOT_IMPLEMENTED | Android SDK/AGP/Gradle/toolchain are separate, version-pinned provider dependencies |
| Google Play App Signing | external Google Play service | Upload key != app-signing key; Play-signed delivered APK must be read back |
| Pure freestanding crypto/toolchain | TOKEN_VAZIO | require source-side test vectors, device/architecture proofs |

## 11 release control stages
1. Identity ownership; 2. license and component inventory; 3. source commit and supply chain; 4. compiler provenance; 5. binary digest; 6. signer identity and offline/HSM custody; 7. APK/AAB and ABI compatibility; 8. privacy and children; 9. adversarial falsifiers; 10. independent review + provider enforcement; 11. release receipt and independent readback.

All stages default FAIL_CLOSED. A green local hash test cannot approve release.

## Commands (untrusted metadata; never a signed assertion)

\`\`\`bash
python3 incubator/RAFAELIA_APK_AUTHORITY/catalog.py record path/to/app.apk \
  --source-commit 0123456789abcdef0123456789abcdef01234567 \
  --author-uri https://github.com/rafaelmeloreisnovo \
  --package-id org.example.app --toolchain-name candidate-compiler \
  --toolchain-sha256 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \
  --out release.candidate.json

python3 incubator/RAFAELIA_APK_AUTHORITY/catalog.py verify path/to/app.apk \
  --manifest release.candidate.json \
  --trusted-author-uri https://github.com/rafaelmeloreisnovo \
  --trusted-source-commit 0123456789abcdef0123456789abcdef01234567 \
  --hash-only

python3 -m unittest discover -s incubator/RAFAELIA_APK_AUTHORITY/tests -v
\`\`\`

Without \`--hash-only\`, verification deliberately returns \`NOT_VERIFIED_SIGNER\` (exit 3). Hash-only \`PASS\` is never permission to publish. Do not put .jks, .p12, private keys, Google credentials, personal/device identifiers, or secrets in source/artifacts.

## Cryptographic lastro / author page

An \`assets/rafaelia.author.json\` identity locator may be included **before APK signing**, with public owner URL, identity key reference, license and build intent. An external **signed, independently retained attestation** must bind the final APK SHA-256 to the author identity key and release/source tree (e.g. DSSE/in-toto), with transparency or independent custody. The Android APK signature protects the installed payload; the Google Play certificate must be verified against Play Console details. A copied APK can always be modified or have attribution text stripped by someone controlling it; tampering must be detectable *against trusted signed evidence* rather than claimed impossible.

\`artifact-level public marker != legally proven authorship\`. Cryptographic attestation on unsigned JSON = TOKEN_VAZIO until signature and trusted identity verification are actually run.

## Authority
See \`governance/CONTROL_MATRIX.md\`, \`governance/SIGNING_BOUNDARY.md\`, \`research/GEOMETRY_ROUTE.md\`. No merge or Google Play upload until independent review, exact-head CI, provider rulesets readback, real APK signing, Play-signed binary readback, privacy/children checks, and legal/license review. The original SETUP MIT license stays unchanged; independent rights require legal evidence.
