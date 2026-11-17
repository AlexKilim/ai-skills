---
name: gitea
description: "Configure a project's existing Gitea issue tracker and read, publish, update, or triage issues and specifications. Use for Gitea tasks, labels, and engineering-skill integration across repositories. Installing or administering the Gitea server is a separate workflow."
---

# Gitea

Work with the current project's Gitea tracker. Keep server addresses, repository names, credentials, label vocabulary, and document language specific to that project.

## Establish the target

- Read existing tracker instructions, especially docs/agents/issue-tracker.md and docs/agents/triage-labels.md, plus relevant root instructions. Use the project's glossary and ADRs when composing specifications.
- Otherwise inspect git remotes and infer the intended repository from the user's request and checkout. Account for origin/upstream forks; clarify only when more than one target remains plausible. Do not replace remotes to configure issue tracking.
- Reuse an existing web URL, preserving its scheme, port and reverse-proxy path. An SSH clone URL alone does not determine the HTTP API address. Discover an advertised web URL or ask for the missing base URL rather than guessing credentials into a different endpoint.
- Verify Gitea at the intended server, then GET the repository through its authenticated API. Confirm full_name, has_issues and the relevant access. A private repository can return 404 to an unauthenticated client; this does not establish that the repository is absent.

## Choose the operation

- For project onboarding or tracker configuration, read [Project setup](references/project-setup.md). Ordinary issue work does not require rewriting project configuration.
- Prefer a connected Gitea tool if it covers the operation. Otherwise use the REST API or an already configured Gitea CLI. Read [API workflow](references/api-workflow.md) for routes, authentication and PowerShell handling.
- For reading or analysis, fetch the issue body, labels and discussion; paginate lists. Issue numbers are scoped to their repository. Distinguish issues from pull requests when listing tasks.
- For publishing, use the material the user authorized. Preserve actual multiline text and the agreed specification structure. A local document alone is not publication. Resolve configured label names to current server IDs; IDs are not portable between projects.
- For updates, preserve unrelated description content, labels and assignments. Use the issue's current state. A tracker skill does not itself authorize posting extra comments, enabling features or changing settings beyond the user's task.

## Verify writes and handle uncertainty

- Use an available saved Git credential for the exact HTTP origin or a token already associated with that server. Read credentials privately and send them in the authorization header. For 2FA or insufficient access, use the configured scoped token flow or report the missing access; do not request secrets in chat or create tokens automatically.
- Check existing labels before creating missing ones. Reuse the project's vocabulary; ready-for-agent is a workflow convention, not a Gitea built-in status. Assign it only when the specification is ready under that project's process.
- After a successful write, read back the issue or labels and verify the intended title, description, state and label changes. Report the returned issue number and html_url.
- A timeout during POST may mean the write succeeded. Before repeating, search recent issues using title, body and context, or inspect the relevant comments or labels. Retry a write at most once, only when read-back establishes that it was not applied. If read-back cannot resolve the outcome, stop mutations and report the uncertainty and prepared local material.
- Authentication or permission failures require fixing access, not repeated writes. Preserve prepared content and report the specific blocking operation when publication cannot be verified.

## Completion

For issue work, return the verified task link and material limitations. For onboarding, report the configuration files and confirmed server changes. Keep implementation work separate unless it was also requested.
