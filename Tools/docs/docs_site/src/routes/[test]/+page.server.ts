import type {PageServerLoad} from './$types';
import {HUH} from "$lib";

export const load: PageServerLoad = async ({params}) => {
    let test = await HUH.Test()
    // console.log(test)

    return {
        post: {
            title: test.compounds[0].name,
            content: `Content for ${params.test} goes here`
        }
    };
};


export const prerender = true;
