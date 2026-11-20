---
name: skill-maintenance
description: Evaluate preferred skills and shared components in use. Propose approval-gated improvements when a practice is outdated, a better fit is available, or development reveals a reusable solution, recurring problem, correction, or conflict.
---

While completing the requested work, notice patterns in evidence already encountered. A correction may be project-specific. A recurring problem can be noticed by the agent without a user prompt. Name the observed occurrences, a concrete second use for a successful solution, or the exact instructions that conflict. If there is no actionable candidate, finish the task normally.

Read only the relevant skill or component. Check whether it already covers the case. Choose the smallest durable change: a conditional rule in its owning skill, a project-local convention, or a reusable implementation in the shared library. Apply the same reasoning to this skill itself. Keep one-off observations in the conversation. Avoid background scans, session journals and routine retrospective reports.

Read the [preferred skills list](references/preferred-skills.md) when choosing skills for a nontrivial task or reviewing the user's preferences. Use it as a starting point and follow the task's requirements and repository instructions. Revisit entries when their scope overlaps, they are superseded, or a better fit emerges. Apply this check to this skill and the shared library as well.

When you encounter evidence that a current rule or implementation may be outdated or that a better practice fits the task, verify the relevant version and constraints against current primary sources or a small comparison. Explain what would improve here, the trade-offs, and which projects the finding applies to. Adoption can help identify alternatives; use concrete benefits and compatibility to judge them. Use evidence gathered for the task and limit further checks to alternatives likely to affect the outcome.

Distinguish adopting a practice for this task from making it a shared default. Once the user approves the practice for the task, propose the precise instruction, library change, or preferred-list entry to update. An explicit request can approve the task and its durable change together when it covers both.

Before editing any personal skill or the preferred skills list, adding a shared widget, or changing a shared library, obtain explicit confirmation covering that change. During ordinary project work, batch proposals at the end: number each item, name its destination, explain the evidence and the proposed change, and offer GitHub publication in the same question. For conflicting instructions, show the applicable conditions and ask the user to choose the intended behavior. Continue independent project work while a proposal is pending.

Apply only the confirmed items. Edit the maintained source, check the original case and a nearby case where the rule should stay inactive, and ensure the installed copy reflects the change. Publish only when authorized. Verify the remote revision after pushing; if the approved work remains local, say so and offer publication. An explicit confirmation covering both edits and publication suffices for both.
