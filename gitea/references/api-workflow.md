# API workflow

Use the installation's API documentation for version-specific features: the web base plus /api/swagger or /swagger.v1.json when available. General reference: https://docs.gitea.com/development/api-usage/ . Preserve reverse-proxy prefixes when constructing both URLs.

## Authentication

Use the exact established HTTP origin. A Git SSH key is not automatically HTTP Basic authentication. Prefer a configured server-specific token or an existing HTTP Git credential. With Gitea access tokens, use Authorization: token VALUE; with a saved username/password pair, use Authorization: Basic BASE64. OAuth bearer tokens follow their configured flow.

For Git credentials, run git credential fill with protocol, host (including the HTTP port), and the appropriate repository path when credential.useHttpPath is used. Disable interactive prompting for unattended lookup. Keep the command's stdout captured internally; print only whether usable credentials were found. Avoid diagnostic dumps of environment variables, credential objects and headers. Restore temporary environment overrides after lookup. The snippets below assume an internally prepared $headers dictionary and $repoApi string, with no secrets embedded in command text.

Authentication may return 404 for private resources. If authenticated access still fails, distinguish repository identity from token scope or account rights before changing settings.

## Common routes

API base: WEB_BASE/api/v1. Repository base: API_BASE/repos/OWNER/REPOSITORY, called $repoApi below. Encode owner/repository path segments appropriately.

| Operation | Method and path relative to repository base | Body or notes |
| --- | --- | --- |
| Verify target | GET empty path | full_name, has_issues, available permissions |
| List tasks | GET /issues | state, type=issues, page, limit, optional labels |
| Read task | GET /issues/INDEX | index/number is not the database id |
| Read discussion | GET /issues/INDEX/comments | paginate |
| Create task/spec | POST /issues | title, body, labels as numeric IDs |
| Edit task | PATCH /issues/INDEX | only intended changed fields; state open/closed |
| Add comment | POST /issues/INDEX/comments | body |
| List labels | GET /labels | paginate |
| Create label | POST /labels | name, color as six hex digits, description |
| Add labels | POST /issues/INDEX/labels | labels as numeric IDs; preserves other labels |
| Remove label | DELETE /issues/INDEX/labels/ID | remove only the selected label |

Use the installed schema for repository-setting edits and less common features. For issue dependencies or hierarchy without a verified native API, use explicit parent/blocker links in the description and a matching child list in the parent. Check blocker state when computing readiness.

## PowerShell: arrays and UTF-8

Invoke-RestMethod can emit an API array as a single pipeline object. Capture the response first, then enumerate it. Otherwise a Where-Object filter can accidentally select an entire collection and yield multiple label IDs where one was expected.

```powershell
$labelIds = @(for ($page = 1; ; $page++) {
    $response = Invoke-RestMethod -Uri ("{0}/labels?page={1}&limit=50" -f $repoApi, $page) -Headers $headers -ErrorAction Stop
    if (@($response).Count -eq 0 -or $null -eq $response) { break }
    foreach ($label in $response) {
        if ($label.name -ceq $configuredLabelName) { [long]$label.id }
    }
})
if ($labelIds.Count -eq 0) { throw "Label not found: $configuredLabelName" }
if ($labelIds.Count -gt 1) { throw "Ambiguous label: $configuredLabelName" }
$labelId = $labelIds[0]
```

Paginate until the server indicates there is no next page. Prefer Link or total-count headers when available; otherwise continue until an empty page. A short page is not sufficient evidence if the server may clamp the requested limit.

Read descriptions from a UTF-8 file or construct a structured body. Encoding JSON to UTF-8 bytes avoids PowerShell 5.1 corrupting non-ASCII text:

```powershell
$bodyText = [System.IO.File]::ReadAllText($bodyFile, [System.Text.Encoding]::UTF8)
$payload = @{title=$title; body=$bodyText; labels=@($labelId)} | ConvertTo-Json -Depth 8
$payloadBytes = [System.Text.Encoding]::UTF8.GetBytes($payload)
$issue = Invoke-RestMethod -Uri ($repoApi + '/issues') -Method Post -Headers $headers -ContentType 'application/json; charset=utf-8' -Body $payloadBytes
$verified = Invoke-RestMethod -Uri ($repoApi + '/issues/' + $issue.number) -Headers $headers
```

Compare the returned title, description and intended labels before reporting publication. Normalize CRLF/LF for comparison if necessary, while preserving text and actual multiline structure. The create snippet is for one authorized POST, not a retry loop.

## Failures and retries

A write timeout is an unknown result. Use read-only lookup to reconcile it before another mutation. Search by title and description, not title alone; include closed issues and paginate. For comments and labels, inspect that resource's current state. If matching outcomes are ambiguous or unavailable, stop and report the unknown result. Do not automatically redirect authenticated requests to a different origin.

If publication is blocked, retain the prepared content in the project's established local location and distinguish that from a published issue. Return the API-confirmed html_url after successful read-back.
