# Security Policy

## Scope

Grade Guard is currently an early Windows console prototype that handles local student-profile data and grade records.
Security concerns may include memory-safety issues, unsafe file handling, CSV parsing flaws, unintended data exposure, or behavior that could corrupt saved records.

## Supported Versions

| Version | Supported |
| --- | --- |
| 0.0.x | yes |
| Unreleased snapshots older than the current repository state | no |

## Reporting a Vulnerability

Use coordinated disclosure.
Do not open public issues for suspected vulnerabilities.

Send reports to `zecalifornia@up.edu.ph` with:

- the affected component, file path, or feature
- reproduction steps or a proof of concept
- impact and severity estimate
- suggested mitigation, if available

## Response Targets

- acknowledgment within 72 hours
- initial triage within 7 calendar days
- best-effort remediation plan within 30 calendar days

Complex issues may take longer, but status updates should be provided when triage or remediation needs more time.

## Safe Testing Expectations

- Avoid privacy violations, destructive testing, or unnecessary data exposure.
- Limit testing to systems and data you are authorized to use.
- Keep reports factual, reproducible, and focused on defensive remediation.

## Disclosure and Credit

- Disclosure timing will be coordinated after a fix or mitigation is available.
- Reporters will be credited unless they request anonymity.
- Public write-ups should wait until maintainers confirm that disclosure is appropriate.
