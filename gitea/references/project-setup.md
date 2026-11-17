# Project setup

Read this when configuring a repository to use Gitea with engineering skills such as to-spec, to-tickets or triage. Those skills need a per-project tracker contract, not changes to their global instructions.

## Discover and settle choices

Inspect root AGENTS.md and CLAUDE.md, docs/agents/, git remotes, existing issue labels, GLOSSARY.md, any GLOSSARY-MAP.md and docs/adr/. Read relevant existing configuration before proposing changes.

Confirm only unsettled decisions: the target repository, label vocabulary, or a genuinely ambiguous domain-document layout. Reuse existing choices and authorization. A normal Firmware/Software split does not by itself require multiple domain contexts.

If triage is used and no vocabulary is configured, propose these defaults together:

| Role | Default Gitea label |
| --- | --- |
| needs-triage | needs-triage |
| needs-info | needs-info |
| ready-for-agent | ready-for-agent |
| ready-for-human | ready-for-human |
| wontfix | wontfix |

Read repository labels before creating any missing configured label. Keep existing colors, descriptions and custom mappings. has_issues=false means issue tracking needs enabling; do so only within the user's authorized setup scope. Prefer a narrow setting update, and verify it. Report a permission limitation rather than installing a second tracker.

## Write the project contract

For a project using docs/agents/, prepare these files; otherwise preserve an equivalent existing layout:

- **issue-tracker.md:** Gitea web URL, API base including any installation prefix, owner/repository, authentication source without secret values, supported issue operations and publication/read-back rules. Make clear that publishing means creating a server issue. Link shared operational guidance to this skill when useful instead of copying a complete API manual.
- **triage-labels.md:** role-to-label mapping, only if the project's workflow uses triage. Label IDs are discovered per server request, not stored as reusable configuration.
- **domain.md:** where the glossary and ADRs live and when to read them. Reuse the current terminology and decisions; surface conflicts with ADRs. If documents are absent, proceed with available context rather than inventing domain content.

Add or update a compact Agent skills section linking these documents. If CLAUDE.md exists, update it; otherwise use an existing AGENTS.md. Ask which to create only when neither exists. Preserve existing instruction blocks and their encoding and line endings; update an existing section in place.

Useful pointer shape, adapted to the project's language:

    ## Agent skills

    ### Issue tracker
    Before reading or publishing tasks, read docs/agents/issue-tracker.md.

    ### Triage labels
    Before applying workflow labels, read docs/agents/triage-labels.md.

    ### Domain docs
    Before code exploration, read docs/agents/domain.md.

Omit the triage pointer when no triage document is needed. Default domain layout is one root glossary plus the existing ADR directory. Keep PRs as a triage request surface off unless the project has chosen otherwise.

If an explicitly invoked setup skill requires draft review, show the concrete proposed changes and follow that requirement. This skill introduces no additional approval round for already authorized setup. Present configuration and all intended server mutations before requesting any approval that remains necessary.

## Finish

Read back the written configuration, check every local pointer, and verify the configured labels on Gitea. Explain that project settings can be edited directly later. Resume an already authorized pending publication using the new configuration; setup alone does not authorize publishing unrelated material.
