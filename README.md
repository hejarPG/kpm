# KPM
برنامه‌ی KPM یک کد کوچک نوشته‌شده به زبان ++C است که کمک می‌کند فرایند اجرای یک کد KPL روی امولاتور Blitz به شکل ساده‌تری صورت پذیرد. متاسفانه در حال حاضر تنها سیستم‌عامل ویندوز پشتیبانی می‌شود.
## نحوه استفاده
1. ابتدا فایل‌های مربوط به برنامه را از این [لینک](https://github.com/hejarPG/kpm/releases/tag/v1.0.0) دانلود کرده و در یک پوشه‌ی مشخص از حالت فشرده خارج کنید. در ادامه فرض می‌شود فایل‌های `kpm.exe`، `DISK` و پوشه‌‌ی `init` در آدرس `[kpm_path]` قرار دارند.
2. به کمک این [راهنما](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/) آدرس `[kpm_path]` را به PATH Environment Variable اضافه کنید. با انجام این کار قادر خواهید بود از هر جایی در ترمینال از KPM استفاده کنید.
3. به پوشه‌ای که می‌خواهید پروژه در آن باشد رفته و ترمینال را باز کنید (می‌توانید از VS Code نیز استفاده کنید).
4. با اجرای دستور `kpm init name` در ترمینال یک پروژه‌ی Hello World ساخته خواهد شد. پارامتر `name` اختیاری است و اگر داده شود یک پوشه‌ی جدید در پوشه‌ی فعلی ساخته شده و پروژه در آن قرار می‌گیرد. همچنین نام پروژه `name` خواهد بود. در غیر این‌صورت، پروژه در پوشه‌ی فعلی ساخته شده و نام آن همان نام پوشه‌ی فعلی است. در ادامه فرض می‌کنیم آدرس پروژه `[prj_path]` است.
5. با اجرای دستور `kpm out` مراحل زیر به ترتیب اتفاق می‌افتند.
  - ابتدا تمام فایل‌هایی که در پوشه‌ی `[prj_path]` یا یکی از زیرپوشه‌های آن قرار دارند در پوشه‌ی `out` داخل `[prj_path]` کپی می‌شود. بدین طریق، فارغ از این‌که شما به چه شکلی پروژه‌ی خود را سازمان‌دهی کرده‌اید، در انتها تمام فایل‌های مربوط به پروژه در کنار هم قرار می‌گیرند.
  - حال تمام فایل‌های مربوط به زبان KPL کامپایل شده و فایل‌های با پسوند `s.` تولید می‌شوند.
  - در این مرحله تمام فایل‌ّای با پسوند `s.` اسمبل شده و فایل‌های `o.` تولید می‌شود.
  - در نهایت تمام فایل‌های با پسوند `o.` توسط لینکر، لینک می‌شوند.
  - اکنون برنامه آماده‌ی اجرا است. بنابراین امولاتور Blitz اجرا شده و برنامه اجرا می‌شود.
