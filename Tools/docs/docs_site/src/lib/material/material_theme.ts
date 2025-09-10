export function Dark(window: any) {
    window.document.body.classList.remove("light", "light-medium-contrast", "light-high-contrast", "dark", "dark-medium-contrast", "dark-high-contrast");
    window.document.body.classList.add('dark');
}

export function Light(window: any) {
    window.document.body.classList.remove("light", "light-medium-contrast", "light-high-contrast", "dark", "dark-medium-contrast", "dark-high-contrast");
    window.document.body.classList.add('light');
}

export function DarkMediumContrast(window: any) {
    window.document.body.classList.remove("light", "light-medium-contrast", "light-high-contrast", "dark", "dark-medium-contrast", "dark-high-contrast");
    window.document.body.classList.add('dark-medium-contrast');
}

export function LightMediumContrast(window: any) {
    window.document.body.classList.remove("light", "light-medium-contrast", "light-high-contrast", "dark", "dark-medium-contrast", "dark-high-contrast");
    window.document.body.classList.add('light-medium-contrast');
}

export function DarkHighContrast(window: any) {
    window.document.body.classList.remove("light", "light-medium-contrast", "light-high-contrast", "dark", "dark-medium-contrast", "dark-high-contrast");
    window.document.body.classList.add('dark-high-contrast');
}

export function LightHighContrast(window: any) {
    window.document.body.classList.remove("light", "light-medium-contrast", "light-high-contrast", "dark", "dark-medium-contrast", "dark-high-contrast");
    window.document.body.classList.add('light-high-contrast');
}

export async function GetPreference(window: any) {
    return await window.cookieStore.get("theme");
}

export async function SetDefaultTheme(window: any, browser: any) {
    if (browser) {
        let theme = await GetPreference(window);
        if (theme !== null){

        }
    }
}