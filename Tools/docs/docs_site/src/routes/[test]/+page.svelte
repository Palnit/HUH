<script lang="ts">
    import type {PageProps} from './$types';

    let {data}: PageProps = $props();
</script>

<p>{data.post.title}</p>
<div>{@html data.post.content}</div>
