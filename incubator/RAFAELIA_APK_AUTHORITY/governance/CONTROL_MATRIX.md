# Normative mapping — scoped implementation candidate

References define acceptance criteria; they do NOT grant ISO/NIST certification. Check edition, clauses, jurisdiction, applicability and evidence with qualified auditors before making a compliance claim.

| Source / domain | Intended control | Evidence gate | Current |
| --- | --- | --- | --- |
| NIST SP 800-218 SSDF | Secure development, review, vulnerability response | reviewed commits, threat model, falsifiers | PLAN_ONLY |
| NIST SP 800-207 Zero Trust | Explicit per-request identity and least privilege | independent policy enforcement readback | TOKEN_VAZIO |
| NIST SP 800-57, 800-53, 800-161r1 | Key lifecycle, audit, supply chain | real HSM/key policy, secure custody, SBOM | TOKEN_VAZIO |
| ISO/IEC 27001:2022 and 27002 | ISMS risk controls | scope + SoA + audit evidence | TOKEN_VAZIO |
| ISO/IEC 27701 / 29100 | Privacy information management | lawful basis, minimization, rights | TOKEN_VAZIO |
| ISO 8000 series | Data quality/master-data provenance | typed source, completeness and measured quality | PLAN_ONLY |
| ISO/IEC 25010 / IEEE 1012 | Quality, verification/validation | reliability, independent V&V | TOKEN_VAZIO |
| RFC 2119 / RFC 8174 | Normative language | explicit MUST/SHOULD distinction | DOCUMENTED |
| RFC 5280 / RFC 3161 | PKI certificates and time-stamp protocol | chain/time verification by approved authority | TOKEN_VAZIO |
| Android APK Signature Schemes and Play App Signing | Signing and update continuity | app/upload cert readback and real APK verify | TOKEN_VAZIO |
| SLSA / in-toto / SPDX or CycloneDX | Provenance, attestation and SBOM | verified signed attestation, complete inventory | TOKEN_VAZIO |
| Brazil LGPD + ECA Digital (Lei 15.211/2025) | Children's best interests, parental supervision, privacy | DPIA/LIA where applicable, age design, family flows, rights tests | TOKEN_VAZIO |
| Google Play Families + Data safety policies | Store policies | Play Console declarations and tests | TOKEN_VAZIO |
| Rights: copyright, licensing, trade secrets | Rights chain and NOTICE preservation | ownership/licensing evidence and independent legal review | TOKEN_VAZIO |

Do not claim universal compliance. Privacy-first by design: no fingerprinting of minors, covert device telemetry, contact harvesting or permanent identifiable tracking; collect only minimum release/source metadata, no runtime user identity. Parental controls require age-appropriate and proportionate design, not hidden surveillance.

**Gating predicate:** release = positive cryptographic verification AND exact-head source/build provenance AND rights/license review AND privacy/children applicability review AND real binary compatibility AND independent review AND provider protection readback; UNKNOWN in any mandatory element -> HOLD.
