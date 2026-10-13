# Signature boundary and custody (P0)

## Assets and protected operations
- RELEASE APP SIGNING KEY: owned by Play App Signing or explicitly delegated non-Play signer; never present in this repository.
- UPLOAD KEY: separate from app signing key, created and stored offline/HSM; public certificate fingerprint may be recorded after independent readback; private .jks/.p12 is NEVER a code artifact.
- AUTHORSHIP ATTESTATION KEY: separate purpose, unique allowed signing identity, public verification key pinned by approved verifier; never substitute APK upload certificate as legal ownership proof.
- COMPILER ARTIFACT SIGNER: distinct key and release intent; must bind compiler executable checksum and source-to-binary build proof, with double-build comparison where feasible.
- CI OIDC identity: scoped, short-lived, no broad PATs or secrets for untrusted PR workflows. Prefer no signing in pull_request events.

## Non-negotiable verification
1. Freeze source commit and toolchain hashes (do not sign moving tags).
2. Ensure license rights and dependencies recorded; do not erase 3rd-party notices.
3. Build unsigned bundle/APK, include minimal public author locator BEFORE final signing.
4. Compute preliminary hashes; generate platform-specific manifest and SBOM; scan permissions, trackers and children's rules.
5. Sign AAB for upload with authorized upload key **outside PR CI**; validate signatures using official platform verifier.
6. Compare upload certificate SHA-256 with known, authorized fingerprint; verify Play App Signing identity for delivered APKs separately.
7. Read back delivered APK or independently downloaded Play artifact and its cert; compare package identifier, version, source mapping, SDK/ABI and trace.
8. Create signed SLSA/in-toto provenance for final artifact hash with trustworthy builder identity, store externally with append-only receipt.
9. Exercise adversarial cases: changed APK bytes; stripped identity locator; altered manifest; wrong signer; swapped app bundle; wrong package/version; outdated SHA; missing privacy/child gate.
10. Require exact-head, independent human review, server enforced branch rulesets and protected publishing environment. If governance API read fails, state UNKNOWN, not BYPASSED.
11. Emit release decision and independent receipt. Restore a prior *signed release*, never rewrite previous attestations.

## Why removable/undetectable attribution is impossible
No static byte sequence can be guaranteed unremovable once an attacker can replace the file. Stronger achievable property: any changed release ceases to match the signed digest and trusted transparency/receipts. APK signature V2/V3/V4 verification and Play Store management are separate from a public author page, license and legal proof of intellectual-property ownership.

## Android vs pure freestanding
APK signing / AAB publication are platform protocols with mandatory external trust roots and Google Play service boundaries. A freestanding, self-authored verifier needs its own cryptographic conformance test vectors, parser safety and third-party review; absence of a library does not establish secure cryptography. No simulated Android signer is shipped.

## Key incidents
Suspected compromise => freeze signing, rotate/revoke as platform permits, request upload key reset where applicable, preserve previous cert lineage and incident record; do not overwrite release history or relabel old artifacts as new.
