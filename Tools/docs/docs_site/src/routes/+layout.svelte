<script lang="ts">
    import '../app.css';
    import '$lib/material/material'

    import favicon from '$lib/assets/favicon.svg';
    import type {LayoutProps} from './$types';
    import {resolve} from '$app/paths';

    import {browser} from "$app/environment";
    import {MaterialTheme} from "$lib/material/material";

    async function test() {
        let value = await MaterialTheme.SetDefaultTheme(window, browser);
    }

    if (browser) {
        if (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches) {
            MaterialTheme.Dark(window)
            console.log(window)
        } else {
            MaterialTheme.Light(window)
        }
    }

    let {data, children}: LayoutProps = $props();

</script>

<svelte:head>
    <link rel="icon" href={favicon}/>
</svelte:head>
{#await test()}
    <md-divider></md-divider>
{:then value}
    <main>
        <div class="container">
            <div class="nav_bar">
                <md-elevation></md-elevation>
                <div class="scroll_div">
                    <ul>
                        {#each data.DoxygenIndex.compounds as tmp_class}
                            <li><a href={resolve(`/${tmp_class.refId}`)}>{tmp_class.name}</a></li>
                        {/each}
                    </ul>
                </div>
            </div>
            <div>
                {@render children?.()}
            </div>
        </div>

    </main>
{/await}

<style>
    .nav_bar {
        display: flex;
        position: relative;
        width: 20%;
        left: 0;
        height: 100vh;
        border-radius: 0 16px 16px 0;
        max-height: 100%;

        background: var(--md-sys-color-surface-container);
        color: var(--md-sys-color-on-surface);
        --md-elevation-level: 3;
    }

    .scroll_div {
        overflow-y: auto;
        overflow-x: hidden;
        scrollbar-color: var(--md-sys-color-primary) transparent;
        scrollbar-width: thin;
    }


    .container {
        display: flex;
        height: 100vh;
        width: 100vw;
        flex-direction: row;
        position: relative;
    }
</style>
