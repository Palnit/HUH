<script lang="ts">
    import '$lib/material/material'
    import {MaterialTheme} from "$lib/material/material";


    function Dark() {
        MaterialTheme.Dark(window)
    }

    function Light() {
        MaterialTheme.Light(window)
    }

</script>

<h1>Welcome to SvelteKit</h1>
<p class="text-amber-300">Visit <a href="https://svelte.dev/docs/kit">svelte.dev/docs/kit</a> to read the documentation
</p>
<!-- svelte-ignore a11y_click_events_have_key_events, a11y_no_static_element_interactions (because of web component) -->
<md-filled-button onclick={Dark}>To Dark</md-filled-button>
<!-- svelte-ignore a11y_click_events_have_key_events, a11y_no_static_element_interactions (because of web component) -->
<md-filled-button onclick={Light}>To Light</md-filled-button>
<a href="/my_test">Home</a>
